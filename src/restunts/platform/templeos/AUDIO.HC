#include "NATIVE.HH"
#include "AUDIO.HH"

// Native implementation of PC15.DRV. The sequencer retains the original
// envelopes, modulation, resource context masks, and voice allocation.
CAudioTimer audio_timers[TEMPLE_PC_TIMER_COUNT];
CAudioChannel audio_channels[TEMPLE_PC_CHANNEL_COUNT];
CAudioChannel *audio_sfx_channels=audio_channels+TEMPLE_PC_EFFECT_FIRST;
CAudioContext dos_audio_contexts[TEMPLE_PC_CONTEXT_STORAGE];
legacy_u8 dos_audio_master_state[TEMPLE_PC_MASTER_STATE_SIZE]={16,0,22};
legacy_u8 dos_audio_driver_data[TEMPLE_PC_DRIVER_DATA_SIZE];
legacy_void *dos_audio_driver_binary=NULL;
legacy_s16 audio_update_lock=AUDIO_UPDATE_LOCKED;
legacy_s8 audio_music_enabled=AUDIO_STATE_ENABLED;
legacy_s8 audio_effects_enabled=AUDIO_STATE_ENABLED;
legacy_u8 dos_audio_uses_direct_channels=FALSE;
legacy_u8 dos_audio_special_mode=FALSE;
legacy_u8 dos_audio_master_volume=0;
legacy_u8 dos_audio_context_count=0;

CTemplePcVoice temple_pc_voices[TEMPLE_PC_CONTEXT_COUNT];
legacy_u8 temple_pc_mix_phase=0;
legacy_u8 temple_pc_initialized=FALSE;
legacy_s8 temple_pc_saved_note=0;
legacy_u8 temple_pc_saved_gate=0;
legacy_u16 temple_pc_output_period=0;
legacy_u8 temple_pc_output_active=FALSE;
CTask *temple_pc_sample_task=NULL;
legacy_u8 *temple_pc_sample_data=NULL;
legacy_u64 temple_pc_sample_length=0;
legacy_u64 temple_pc_sample_position=0;
legacy_u64 temple_pc_sample_step=0;
legacy_u8 temple_pc_sample_active=FALSE;
legacy_u8 temple_pc_sample_busy=FALSE;
legacy_u8 temple_pc_sample_shutdown=FALSE;

// HolyC does not zero uninitialized global allocations.
MemSet(audio_timers,0,sizeof(audio_timers));
MemSet(audio_channels,0,sizeof(audio_channels));
MemSet(dos_audio_contexts,0,sizeof(dos_audio_contexts));
MemSet(dos_audio_driver_data,0,sizeof(dos_audio_driver_data));
MemSet(temple_pc_voices,0,sizeof(temple_pc_voices));

// Exact PIT divisors at PC15.DRV:003F and PCM rates at PC15.DRV:012F.
// These tables preserve the DOS pitches; Snd() would quantize engine pitch.
legacy_u16 temple_pc_note_period[TEMPLE_PC_NOTE_COUNT]={0,0,65009,61360,57917,54666,51598,
    48702,45968,43388,40953,38655,36485,34437,32505,30680,28958,27333,25799,24351,
    22984,21694,20477,19327,18243,17219,16252,15340,14479,13666,12899,12175,11492,
    10847,10238,9664,9121,8609,8126,7670,7240,6833,6450,6088,5746,5424,5119,4832,
    4561,4305,4063,3835,3620,3417,3225,3044,2873,2712,2560,2416,2280,2152,2032,1918,
    1810,1708,1612,1522,1437,1356,1280,1208,1140,1076,1016,959,905,854,806,761,718,
    678,640,604,570,538,508,479,452,427,403,380,359,339,320,302,285,269,254,240,
    226,214,202,190,180,169,160,151,143,135,127,120,113,107,101,95,90,85,80,75};

legacy_u16 temple_pc_sample_rate[TEMPLE_PC_SAMPLE_NOTE_COUNT]={1047,1109,1175,1245,1319,1397,
    1480,1568,1661,1760,1865,1976,2093,2217,2349,2489,2637,2794,2960,3136,3322,3520,
    3729,3951,4186,4435,4699,4978,5274,5588,5920,6272,6645,7040,7459,7902,8372,8870,
    9397,9956,10548,11175,11840,12544,13290,14080,14917,15804,16744,17740,18795,19912,
    21096,22351,23680,25088,26580,28160,29834,31609};

legacy_void TemplePcSilence()
{
  OutU8(TEMPLE_PC_GATE_PORT,InU8(TEMPLE_PC_GATE_PORT)&~TEMPLE_PC_GATE_BITS);
  temple_pc_output_active=FALSE;
}

legacy_void TemplePcOutput(legacy_u16 period)
{
  // PIT count zero is a full 65536-clock cycle, as in the DOS driver.
  if(!temple_pc_output_active||temple_pc_output_period!=period) {
    OutU8(TEMPLE_PC_PIT_CONTROL,TEMPLE_PC_MODE_SQUARE);
    OutU8(TEMPLE_PC_PIT_COUNTER,period&TEMPLE_BYTE_MASK);
    OutU8(TEMPLE_PC_PIT_COUNTER,period>>TEMPLE_BYTE_BITS);
    OutU8(TEMPLE_PC_GATE_PORT,InU8(TEMPLE_PC_GATE_PORT)|TEMPLE_PC_GATE_BITS);
    temple_pc_output_period=period;
    temple_pc_output_active=TRUE;
  }
}

legacy_u16 TemplePcNotePeriod(legacy_s64 note)
{
  note&=TEMPLE_BYTE_MASK;
  if(note>=TEMPLE_PC_NOTE_COUNT) {
    return 0;
  }
  return temple_pc_note_period[note];
}

legacy_void TemplePcStorePeriod(CAudioContext *context,CTemplePcVoice *voice)
{
  if(context) {
    context->driver_private_state[TEMPLE_PC_DRIVER_NOTE_OFFSET]=voice->note;
    context->driver_private_state[TEMPLE_PC_DRIVER_BASE_LOW]=voice->base_period;
    context->driver_private_state[TEMPLE_PC_DRIVER_BASE_HIGH]=
        voice->base_period>>TEMPLE_BYTE_BITS;
    context->driver_private_state[TEMPLE_PC_DRIVER_CURRENT_LOW]=voice->period;
    context->driver_private_state[TEMPLE_PC_DRIVER_CURRENT_HIGH]=voice->period>>TEMPLE_BYTE_BITS;
  }
}

legacy_void TemplePcStopSample()
{
  temple_pc_sample_active=FALSE;
  while(temple_pc_sample_busy) {
    Yield;
  }
  if(temple_pc_sample_data) {
    Free(temple_pc_sample_data);
    temple_pc_sample_data=NULL;
  }
  temple_pc_voices[TEMPLE_PC_VOICE_SAMPLE].active=FALSE;
  temple_pc_output_active=FALSE;
  OutU8(TEMPLE_PC_PIT_CONTROL,TEMPLE_PC_MODE_SQUARE);
}

// The DOS driver replaces IRQ0 for its 17 kHz PWM stream. A native task keeps
// the OS timer intact. A spare core supplies precise timing when available;
// on a single core the task yields every short batch so the game can advance.
legacy_void TemplePcSampleTask(legacy_u8 *unused)
{
  legacy_u64 index,frames=0;
  legacy_u8 pulse,gate;
  legacy_f64 next_sample=tS;
  legacy_f64 interval=TEMPLE_PC_SAMPLE_FRAME/ToF64(TEMPLE_PC_PIT_HZ);
  while(!temple_pc_sample_shutdown) {
    if(!temple_pc_sample_active) {
      temple_pc_sample_busy=FALSE;
      Sleep(1);
      next_sample=tS;
    } else {
      temple_pc_sample_busy=TRUE;
      if(temple_pc_sample_active) {
        temple_pc_sample_position+=temple_pc_sample_step;
        index=temple_pc_sample_position>>TEMPLE_PC_SAMPLE_FRACTION_BITS;
        if(index>=temple_pc_sample_length) {
          temple_pc_sample_active=FALSE;
          temple_pc_voices[TEMPLE_PC_VOICE_SAMPLE].active=FALSE;
          TemplePcSilence;
        } else {
          pulse=TEMPLE_PC_SAMPLE_PULSE_MINIMUM+
              (temple_pc_sample_data[index]*
                  (TEMPLE_PC_SAMPLE_FRAME-TEMPLE_PC_SAMPLE_PULSE_MINIMUM))/
                  TEMPLE_BYTE_MASK;
          OutU8(TEMPLE_PC_PIT_COUNTER,pulse);
          OutU8(TEMPLE_PC_PIT_COUNTER,0);
          gate=InU8(TEMPLE_PC_GATE_PORT)|TEMPLE_PC_GATE_BITS;
          OutU8(TEMPLE_PC_GATE_PORT,gate&~TEMPLE_PC_GATE_CLOCK);
          OutU8(TEMPLE_PC_GATE_PORT,gate);
          next_sample+=interval;
          while(temple_pc_sample_active&&tS<next_sample) {
            PAUSE;
          }
          frames++;
          if(mp_cnt==1&&!(frames%TEMPLE_PC_SAMPLE_YIELD_FRAMES)) {
            Yield;
          }
        }
      }
      temple_pc_sample_busy=FALSE;
    }
  }
  temple_pc_sample_busy=FALSE;
  temple_pc_sample_task=NULL;
}

legacy_void TemplePcStartSample(legacy_u8 *resource,legacy_u8 note)
{
  legacy_u8 *sample;
  legacy_u64 size;
  legacy_s64 sample_cpu=-1;
  TemplePcStopSample;
  if(!resource||note>=TEMPLE_PC_SAMPLE_NOTE_COUNT) {
    return;
  }
  sample=RsAudioReadFarPointer(resource+TEMPLE_PC_SAMPLE_REFERENCE_OFFSET);
  if(!sample) {
    return;
  }
  size=sample[TEMPLE_PC_SAMPLE_LENGTH_OFFSET]|
      (sample[TEMPLE_PC_SAMPLE_LENGTH_OFFSET+1]<<TEMPLE_BYTE_BITS)|
      (sample[TEMPLE_PC_SAMPLE_LENGTH_OFFSET+2]<<(TEMPLE_BYTE_BITS*2))|
      (sample[TEMPLE_PC_SAMPLE_LENGTH_OFFSET+3]<<(TEMPLE_BYTE_BITS*3));
  if(!size||size>TEMPLE_PC_SAMPLE_MAXIMUM) {
    return;
  }
  temple_pc_sample_data=MAlloc(size);
  MemCpy(temple_pc_sample_data,sample+TEMPLE_PC_SAMPLE_DATA_OFFSET,size);
  temple_pc_sample_length=size;
  temple_pc_sample_position=0;
  temple_pc_sample_step=
      (temple_pc_sample_rate[note]<<TEMPLE_PC_SAMPLE_FRACTION_BITS)/TEMPLE_PC_SAMPLE_RATE;
  OutU8(TEMPLE_PC_PIT_CONTROL,TEMPLE_PC_MODE_PULSE);
  OutU8(TEMPLE_PC_GATE_PORT,InU8(TEMPLE_PC_GATE_PORT)|TEMPLE_PC_GATE_BITS);
  if(!temple_pc_sample_task) {
    if(mp_cnt>1) {
      sample_cpu=1;
    }
    temple_pc_sample_shutdown=FALSE;
    temple_pc_sample_task=Spawn(&TemplePcSampleTask,NULL,"Restunts Speaker",sample_cpu,Fs);
  }
  temple_pc_voices[TEMPLE_PC_VOICE_SAMPLE].active=TRUE;
  temple_pc_sample_active=TRUE;
}

legacy_u8 RsDosAudioDriverInitialize()
{
  legacy_u64 index;
  if(!temple_pc_initialized) {
    temple_pc_saved_note=scrncast.ona;
    temple_pc_saved_gate=InU8(TEMPLE_PC_GATE_PORT)&TEMPLE_PC_GATE_BITS;
    temple_pc_initialized=TRUE;
  }
  TemplePcStopSample;
  TemplePcSilence;
  MemSet(temple_pc_voices,0,sizeof(temple_pc_voices));
  for(index=0;index<TEMPLE_PC_DEFAULT_VOLUME_COUNT;index++) {
    temple_pc_voices[index].volume=TEMPLE_PC_MAX_VOLUME;
  }
  temple_pc_mix_phase=0;
  return TEMPLE_PC_CONTEXT_COUNT;
}

legacy_void RsDosAudioDriverLoadBank(legacy_void *bank)
{
  // PC15.DRV has no external instrument bank.
}

legacy_void RsDosAudioDriverPrepareContext(
    legacy_s16 channel,CAudioContext *context,legacy_u8 *timer,legacy_void *resource)
{
  if(0<=channel<TEMPLE_PC_CONTEXT_COUNT) {
    temple_pc_voices[channel].channel=timer(CAudioChannel *);
  }
}

legacy_void RsDosAudioDriverSetContextValue(
    legacy_s16 channel,CAudioContext *context,legacy_u16 value)
{
  CTemplePcVoice *voice;
  if(!(0<channel<TEMPLE_PC_CONTEXT_COUNT)) {
    return;
  }
  voice=&temple_pc_voices[channel];
  voice->base_period=0;
  if(value>TEMPLE_PC_ENGINE_PITCH_MINIMUM) {
    voice->base_period=(TEMPLE_PC_PIT_HZ*TEMPLE_PC_ENGINE_PITCH_SCALE)/value;
  }
  voice->period=voice->base_period;
  TemplePcStorePeriod(context,voice);
}

legacy_void RsDosAudioDriverSetControl(
    legacy_s16 channel,CAudioContext *context,legacy_u16 control,legacy_u16 value)
{
  // The original PC driver ignores controller messages.
}

legacy_void RsDosAudioDriverSetPitch(legacy_u8 *timer,legacy_s16 pitch,legacy_s16 channel)
{
  // Channel bend is read from AUDIO_CHANNEL during each context update.
}

legacy_void RsDosAudioDriverSendData(legacy_u16 length,legacy_u8 *data)
{
  // PC15.DRV has no raw command stream.
}

legacy_void RsDosAudioDriverActivateContext(legacy_s16 channel,CAudioContext *context,
    legacy_u8 *timer,legacy_s16 pitch,legacy_u16 parameter,legacy_void *resource)
{
  CTemplePcVoice *voice;
  if(!(0<=channel<TEMPLE_PC_CONTEXT_COUNT)) {
    return;
  }
  voice=&temple_pc_voices[channel];
  voice->channel=timer(CAudioChannel *);
  voice->note=pitch&TEMPLE_BYTE_MASK;
  if(!channel) {
    TemplePcStartSample(resource,voice->note);
  } else {
    if(voice->note!=TEMPLE_PC_NOTE_KEEP) {
      voice->base_period=TemplePcNotePeriod(voice->note);
      voice->period=voice->base_period;
    }
    voice->active=TRUE;
  }
  TemplePcStorePeriod(context,voice);
}

legacy_void RsDosAudioDriverReleaseChannel(legacy_s16 channel)
{
  if(0<=channel<TEMPLE_PC_CONTEXT_COUNT) {
    if(!channel) {
      TemplePcStopSample;
    }
    temple_pc_voices[channel].active=FALSE;
  }
}

legacy_void RsDosAudioDriverStartContext(legacy_s16 channel,CAudioContext *context)
{
  // Release envelopes remain active until end_context, as in PC15.DRV.
}

legacy_void RsDosAudioDriverEndContext(legacy_s16 channel,CAudioContext *context)
{
  RsDosAudioDriverReleaseChannel(channel);
}

legacy_void RsDosAudioDriverReset()
{
  // No reset command is implemented by PC15.DRV.
}

legacy_void RsDosAudioDriverStart()
{
  legacy_u64 channel;
  TemplePcStopSample;
  for(channel=0;channel<TEMPLE_PC_CONTEXT_COUNT;channel++) {
    temple_pc_voices[channel].active=FALSE;
  }
  TemplePcSilence;
}

legacy_void RsDosAudioDriverSuspendContext(legacy_s16 channel,CAudioContext *context,
    legacy_u16 timer_offset,legacy_void *resource_pointer)
{
  CTemplePcVoice *voice;
  legacy_u8 *resource=resource_pointer;
  legacy_s64 period,note,detune,pitch=0,difference,magnitude,product,adjustment;
  if(!(0<channel<TEMPLE_PC_CONTEXT_COUNT)||!context||!context->state||!resource) {
    return;
  }
  voice=&temple_pc_voices[channel];
  note=voice->note;
  period=voice->base_period;
  if(resource[TEMPLE_PC_RESOURCE_SEQUENCE_MODE]==1) {
    note=(note+context->sequence_value)&TEMPLE_BYTE_MASK;
    period=TemplePcNotePeriod(note);
  }
  detune=resource[TEMPLE_PC_RESOURCE_DETUNE];
  if(detune>I8_MAX) {
    detune-=TEMPLE_BYTE_MASK+1;
  }
  period=(period-detune)&TEMPLE_WORD_MASK;
  if(voice->channel) {
    pitch=voice->channel->pitch;
    if(pitch>I16_MAX) {
      pitch-=TEMPLE_WORD_MASK+1;
    }
  }
  if(pitch) {
    magnitude=pitch;
    if(pitch>0) {
      difference=(period-TemplePcNotePeriod(note+resource[TEMPLE_PC_RESOURCE_BEND_RANGE]))&
          TEMPLE_WORD_MASK;
      if(magnitude>=TEMPLE_PC_POSITIVE_BEND_LIMIT) {
        magnitude+=TEMPLE_PC_POSITIVE_BEND_ADJUSTMENT;
      }
    } else {
      magnitude=-pitch;
      difference=(TemplePcNotePeriod(note-resource[TEMPLE_PC_RESOURCE_BEND_RANGE])-period)&
          TEMPLE_WORD_MASK;
    }
    product=(difference*magnitude)&U32_MAX;
    if(product>I32_MAX) {
      product-=(ToI64(U32_MAX)+1);
    }
    adjustment=(product>>TEMPLE_PC_PITCH_FRACTION_BITS)&TEMPLE_WORD_MASK;
    if(pitch>0) {
      period-=adjustment;
    } else {
      period+=adjustment;
    }
  }
  if(resource[TEMPLE_PC_RESOURCE_MODULATION_MODE]==2) {
    period+=context->modulation;
  }
  if(resource[TEMPLE_PC_RESOURCE_ENVELOPE_MODE]==2) {
    period+=context->level;
  }
  voice->period=period&TEMPLE_WORD_MASK;
  TemplePcStorePeriod(context,voice);
}

legacy_void RsDosAudioDriverSuspendAll(CAudioContext *contexts)
{
  legacy_s64 selected=TEMPLE_PC_VOICE_SAMPLE;
  legacy_u8 active[TEMPLE_PC_CONTEXT_COUNT];
  legacy_u64 channel;
  for(channel=0;channel<TEMPLE_PC_CONTEXT_COUNT;channel++) {
    active[channel]=temple_pc_voices[channel].active&&temple_pc_voices[channel].volume;
  }
  if(!temple_pc_sample_active) {
    if(active[TEMPLE_PC_VOICE_EFFECT_SECOND]) {
      selected=TEMPLE_PC_VOICE_EFFECT_SECOND;
    } else if(active[TEMPLE_PC_VOICE_EFFECT_FIRST]) {
      selected=TEMPLE_PC_VOICE_EFFECT_FIRST;
    } else if((active[TEMPLE_PC_VOICE_HIGH_FIRST]||active[TEMPLE_PC_VOICE_HIGH_SECOND])&&
        (!(temple_pc_mix_phase&TEMPLE_PC_MIX_PAIR_PHASE)||
            !(active[TEMPLE_PC_VOICE_LOW_FIRST]||active[TEMPLE_PC_VOICE_LOW_SECOND]))) {
      if(active[TEMPLE_PC_VOICE_HIGH_FIRST]) {
        selected=TEMPLE_PC_VOICE_HIGH_FIRST;
      } else {
        selected=TEMPLE_PC_VOICE_HIGH_SECOND;
      }
    } else if(active[TEMPLE_PC_VOICE_LOW_FIRST]) {
      selected=TEMPLE_PC_VOICE_LOW_FIRST;
    } else if(active[TEMPLE_PC_VOICE_LOW_SECOND]) {
      selected=TEMPLE_PC_VOICE_LOW_SECOND;
    }
    if(selected) {
      TemplePcOutput(temple_pc_voices[selected].period);
    } else {
      TemplePcSilence;
    }
  }
  temple_pc_mix_phase++;
}

legacy_void RsDosAudioSetChannelVolume(legacy_s16 channel,legacy_s16 volume)
{
  legacy_u64 index;
  if(!(0<=channel<TEMPLE_PC_CHANNEL_COUNT)) {
    return;
  }
  audio_channels[channel].volume=volume&TEMPLE_BYTE_MASK;
  for(index=0;index<TEMPLE_PC_CONTEXT_COUNT;index++) {
    if(dos_audio_contexts[index].channel==channel) {
      temple_pc_voices[index].volume=volume&TEMPLE_BYTE_MASK;
    }
  }
}

legacy_void RsDosAudioBindChannelContext(legacy_s16 channel,legacy_void *resource_pointer)
{
  legacy_u8 *resource=resource_pointer;
  legacy_u64 index;
  CAudioChannel *state;
  if(!(0<=channel<TEMPLE_PC_CHANNEL_COUNT)||!resource) {
    return;
  }
  state=&audio_channels[channel];
  RsAudioWriteFarPointer(&state->resource,resource);
  state->driver_channel=(channel&(TEMPLE_PC_EFFECT_FIRST-1))+1;
  if(resource[TEMPLE_PC_RESOURCE_DIRECT_CHANNEL]<TEMPLE_PC_EFFECT_FIRST) {
    state->driver_channel=resource[TEMPLE_PC_RESOURCE_DIRECT_CHANNEL];
  }
  for(index=0;index<TEMPLE_PC_CONTEXT_COUNT;index++) {
    if(dos_audio_contexts[index].channel==channel) {
      RsDosAudioDriverPrepareContext(index,&dos_audio_contexts[index],state,resource);
    }
  }
}

legacy_void RsDosAudioSetContextPitch(legacy_s16 context_index,legacy_s16 pitch)
{
  CAudioContext *context;
  if(0<=context_index<TEMPLE_PC_CONTEXT_COUNT) {
    context=&dos_audio_contexts[context_index];
    RsDosAudioDriverSetContextValue(context->driver_channel,context,pitch&TEMPLE_WORD_MASK);
  }
}

legacy_void RsDosAudioDriverSetMasterState(legacy_s16 operation,legacy_void *state)
{
  // The PC driver has no master-volume command; channel volume gates voices.
}

legacy_void TempleAudioTick()
{
  // Mixing occurs exactly once per original audio_update_driver_contexts().
}

legacy_void TempleAudioShutdown()
{
  TemplePcStopSample;
  temple_pc_sample_shutdown=TRUE;
  while(temple_pc_sample_task) {
    Yield;
  }
  TemplePcSilence;
  if(temple_pc_initialized) {
    Snd;
    if(temple_pc_saved_gate&&temple_pc_saved_note) {
      Snd(temple_pc_saved_note);
    }
    temple_pc_initialized=FALSE;
  }
}

legacy_void RsDosAudioShutdown()
{
  audio_update_lock=AUDIO_UPDATE_LOCKED;
  if(dos_audio_driver_binary) {
    RsTimerRemoveCallback(&RsAudioSequenceTimer);
  }
  audio_music_enabled=AUDIO_STATE_DISABLED;
  audio_effects_enabled=AUDIO_STATE_DISABLED;
  if(temple_pc_initialized||temple_pc_sample_task||temple_pc_sample_data) {
    RsDosAudioDriverStart;
    TempleAudioShutdown;
  }
  dos_audio_driver_binary=NULL;
  dos_audio_uses_direct_channels=FALSE;
  dos_audio_special_mode=FALSE;
  dos_audio_context_count=0;
  audio_update_lock=AUDIO_UPDATE_UNLOCKED;
}
