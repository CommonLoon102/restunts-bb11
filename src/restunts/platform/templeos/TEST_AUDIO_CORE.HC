// Run from a fresh task. Exercises the real translated packed-pointer mapper.
#include "LOAD.HC"

#define TEMPLE_AUDIO_CORE_CANARY 0xA5
#define TEMPLE_AUDIO_CORE_CHANNEL 0

legacy_void TempleAudioCoreCallback()
{
}

legacy_void TempleAudioCoreAssert(legacy_u8 condition,legacy_u8 *message)
{
  if(!condition) {
    PrintErr("Audio core test: %s\n",message);
    throw('Audio');
  }
}

legacy_void TempleAudioCoreTest()
{
  legacy_u8 *bytes=audio_channels(legacy_u8 *);
  legacy_u8 *field=(&audio_channels[TEMPLE_AUDIO_CORE_CHANNEL].finish_callback)(legacy_u8 *);
  legacy_u8 *callback=(&TempleAudioCoreCallback)(legacy_u8 *);
  legacy_u8 *saved=MAlloc(sizeof(audio_channels));
  legacy_u64 index,field_offset=field-bytes;
  MemCpy(saved,bytes,sizeof(audio_channels));
  MemSet(bytes,TEMPLE_AUDIO_CORE_CANARY,sizeof(audio_channels));
  try {
    RsAudioSetFinishCallback(TEMPLE_AUDIO_CORE_CHANNEL,callback);
    TempleAudioCoreAssert(
        RsAudioReadFarPointer(field)==callback,"native function pointer round trip");
    for(index=0;index<sizeof(audio_channels);index++) {
      if(index<field_offset||index>=field_offset+sizeof(CAudioFarPointer)) {
        TempleAudioCoreAssert(bytes[index]==TEMPLE_AUDIO_CORE_CANARY,
            "packed callback write preserves neighboring fields and channels");
      }
    }
    RsAudioSetFinishCallback(TEMPLE_AUDIO_CORE_CHANNEL,NULL);
    TempleAudioCoreAssert(
        RsAudioReadFarPointer(field)==NULL,"cleared function pointer round trip");
  } catch {
    MemCpy(bytes,saved,sizeof(audio_channels));
    Free(saved);
    // An uncaught HolyC exception continues unwinding after this cleanup.
    Fs->catch_except=FALSE;
  }
  MemCpy(bytes,saved,sizeof(audio_channels));
  Free(saved);
  Print("Audio core: callback mapping and neighboring channel preservation passed.\n");
}

TempleAudioCoreTest;
