#include "NATIVE.HH"

#define TEMPLE_PRESENT_INTERVAL (1.0/60.0)

legacy_u8 temple_pumping=FALSE;
legacy_s16 temple_exit_status=0;
legacy_f64 temple_last_present=0;

legacy_void TemplePump()
{
  legacy_f64 now;
  if(temple_pumping) {
    return;
  }
  temple_pumping=TRUE;
  // TempleOS schedules cooperatively. The window manager queues input and
  // displays frames even when rendering has consumed a complete timer tick.
  Yield;
  TempleInputPump();
  TempleTimerPump();
  now=tS;
  if(now-temple_last_present>=TEMPLE_PRESENT_INTERVAL) {
    temple_last_present=now;
    TempleVideoPresent();
  }
  temple_pumping=FALSE;
}

legacy_u8 *RsInternal1Memcpy(legacy_u8 *destination,legacy_u8 *source,legacy_u16 length)
{
  return MemCpy(destination,source,length);
}

legacy_u8 *RsInternal2Fmemcpy(legacy_u8 *destination,legacy_u8 *source,legacy_u16 length)
{
  return MemCpy(destination,source,length);
}

legacy_s16 RsDosWriteStdout(legacy_s8 *text,legacy_u16 length)
{
  legacy_u16 index;
  for(index=0;index<length;index++) {
    PutChars(text[index]);
  }
  return length;
}

legacy_s16 RsDosWriteStderr(legacy_s8 *text,legacy_u16 length)
{
  return RsDosWriteStdout(text,length);
}

legacy_void RsDosShowStartupError(legacy_s8 *message)
{
  Print("%s",message);
}

legacy_void RsDosProcessExit(legacy_s16 status)
{
  temple_exit_status=status;
  throw(TEMPLE_EXIT_EXCEPTION);
}

legacy_void RsHeadlessExit(legacy_s16 status)
{
  RsDosProcessExit(status);
}

legacy_s16 RsDosDataStackSegmentsMatch()
{
  return TRUE;
}

legacy_void RsDosInstallDivideErrorHandler()
{
  // The translated game's legacy division functions implement DOS recovery.
}

legacy_void RsDosInterruptsDisable()
{
  // Callbacks execute synchronously; never mask TempleOS hardware interrupts.
}

legacy_void RsDosInterruptsEnable()
{
}

legacy_void RsDosSetCriticalErrorHandler(legacy_u8 *callback)
{
  // File operations return DOS error codes through dos_file_error().
}
