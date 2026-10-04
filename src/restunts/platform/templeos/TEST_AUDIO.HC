// Run in a fresh task. This isolates AUDIO.HC from the game sequencer and
// supplies one synthetic PCM resource; production code uses the real mapper.
#include "LEGACY.HH"
#include "generated/TYPES.HH"

#define TEMPLE_AUDIO_TEST_FULL_PERIOD 0
#define TEMPLE_AUDIO_TEST_NOTE 60
#define TEMPLE_AUDIO_TEST_PERIOD 2280
#define TEMPLE_AUDIO_TEST_ENGINE_VALUE 1600
#define TEMPLE_AUDIO_TEST_ENGINE_PERIOD 11931
#define TEMPLE_AUDIO_TEST_BEND 4096
#define TEMPLE_AUDIO_TEST_BEND_RANGE 2
#define TEMPLE_AUDIO_TEST_BENT_PERIOD 2153
#define TEMPLE_AUDIO_TEST_RESOURCE_SIZE 93
#define TEMPLE_AUDIO_TEST_SAMPLE_BYTES 1000
#define TEMPLE_AUDIO_TEST_SAMPLE_HEADER 50
#define TEMPLE_AUDIO_TEST_SAMPLE_NOTE 30
#define TEMPLE_AUDIO_TEST_AUDIBLE_MS 150
#define TEMPLE_AUDIO_TEST_SAMPLE_TIMEOUT 2.0

legacy_u8 *temple_audio_test_sample=NULL;

legacy_u8 *RsAudioReadFarPointer(legacy_u8 *source)
{
  no_warn source;
  return temple_audio_test_sample;
}

legacy_void RsAudioWriteFarPointer(legacy_u8 *destination,legacy_u8 *value)
{
  no_warn destination,value;
}

legacy_void RsAudioSequenceTimer()
{
}

legacy_void RsTimerRemoveCallback(legacy_void (*callback)())
{
  no_warn callback;
}

#include "AUDIO.HC"

legacy_void TempleAudioTestAssert(legacy_u8 condition,legacy_u8 *message)
{
  if(!condition) {
    PrintErr("PC speaker test: %s\n",message);
    throw('PcAudio');
  }
}

legacy_void TempleAudioTests()
{
  CAudioContext context;
  CAudioChannel channel;
  legacy_u8 resource[TEMPLE_AUDIO_TEST_RESOURCE_SIZE];
  legacy_u8 sample[TEMPLE_AUDIO_TEST_SAMPLE_HEADER+TEMPLE_AUDIO_TEST_SAMPLE_BYTES];
  legacy_f64 deadline;
  MemSet(&context,0,sizeof(CAudioContext));
  MemSet(&channel,0,sizeof(CAudioChannel));
  MemSet(resource,0,sizeof(resource));
  MemSet(sample,TEMPLE_BYTE_MASK/2,sizeof(sample));
  try {
    TempleAudioTestAssert(RsDosAudioDriverInitialize()==TEMPLE_PC_CONTEXT_COUNT,
        "original seven-context interface");
    TempleAudioTestAssert(TemplePcNotePeriod(TEMPLE_AUDIO_TEST_NOTE)==TEMPLE_AUDIO_TEST_PERIOD,
        "original note divisor");
    context.state=AUDIO_STATE_ENABLED;
    RsDosAudioDriverSetContextValue(
        TEMPLE_PC_VOICE_LOW_FIRST,&context,TEMPLE_AUDIO_TEST_ENGINE_VALUE);
    TempleAudioTestAssert(
        temple_pc_voices[TEMPLE_PC_VOICE_LOW_FIRST].base_period==TEMPLE_AUDIO_TEST_ENGINE_PERIOD,
        "continuous engine pitch");
    RsDosAudioDriverActivateContext(
        TEMPLE_PC_VOICE_LOW_FIRST,&context,&channel,TEMPLE_PC_NOTE_KEEP,0,resource);
    RsDosAudioDriverSuspendContext(TEMPLE_PC_VOICE_LOW_FIRST,&context,0,resource);
    TempleAudioTestAssert(
        temple_pc_voices[TEMPLE_PC_VOICE_LOW_FIRST].period==TEMPLE_AUDIO_TEST_ENGINE_PERIOD,
        "driver-value note retains pitch");
    RsDosAudioDriverActivateContext(
        TEMPLE_PC_VOICE_LOW_FIRST,&context,&channel,TEMPLE_AUDIO_TEST_NOTE,0,resource);
    resource[TEMPLE_PC_RESOURCE_BEND_RANGE]=TEMPLE_AUDIO_TEST_BEND_RANGE;
    channel.pitch=TEMPLE_AUDIO_TEST_BEND;
    RsDosAudioDriverSuspendContext(TEMPLE_PC_VOICE_LOW_FIRST,&context,0,resource);
    TempleAudioTestAssert(
        temple_pc_voices[TEMPLE_PC_VOICE_LOW_FIRST].period==TEMPLE_AUDIO_TEST_BENT_PERIOD,
        "DOS positive-bend rounding");
    TemplePcOutput(TEMPLE_AUDIO_TEST_FULL_PERIOD);
    TempleAudioTestAssert(
        temple_pc_output_active&&temple_pc_output_period==TEMPLE_AUDIO_TEST_FULL_PERIOD,
        "zero divisor retains the full PIT cycle");
    RsDosAudioDriverSuspendAll(NULL);
    Sleep(TEMPLE_AUDIO_TEST_AUDIBLE_MS);
    sample[TEMPLE_PC_SAMPLE_LENGTH_OFFSET]=TEMPLE_AUDIO_TEST_SAMPLE_BYTES&TEMPLE_BYTE_MASK;
    sample[TEMPLE_PC_SAMPLE_LENGTH_OFFSET+1]=TEMPLE_AUDIO_TEST_SAMPLE_BYTES>>TEMPLE_BYTE_BITS;
    sample[TEMPLE_PC_SAMPLE_LENGTH_OFFSET+2]=0;
    sample[TEMPLE_PC_SAMPLE_LENGTH_OFFSET+3]=0;
    temple_audio_test_sample=sample;
    TemplePcStartSample(resource,TEMPLE_AUDIO_TEST_SAMPLE_NOTE);
    deadline=tS+TEMPLE_AUDIO_TEST_SAMPLE_TIMEOUT;
    while(temple_pc_sample_active&&tS<deadline) {
      Sleep(1);
    }
    TempleAudioTestAssert(
        !temple_pc_sample_active&&temple_pc_sample_position,"native PWM sample reaches its end");
  } catch {
    TempleAudioShutdown;
    // An uncaught HolyC exception continues unwinding after this cleanup.
    Fs->catch_except=FALSE;
  }
  TempleAudioShutdown;
  TempleAudioTestAssert(
      !temple_pc_sample_task&&!temple_pc_sample_data,"sample task and allocation released");
  Print("PC speaker tests passed.\n");
}

TempleAudioTests;
