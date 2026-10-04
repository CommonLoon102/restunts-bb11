#include "NATIVE.HH"

#define TEMPLE_TIMER_CALLBACK_COUNT 5
#define TEMPLE_TIMER_TICKS_PER_SECOND 100
#define TEMPLE_TIMER_SLOW_DIVIDER 5
#define TEMPLE_TIMER_POLL_MS 1

legacy_u8 *temple_timer_callbacks[TEMPLE_TIMER_CALLBACK_COUNT];
legacy_u64 temple_timer_last_tick=0;
legacy_u32 temple_timer_counter=0,temple_timer_realtime=0,temple_timer_previous=0;
legacy_u32 temple_timer_slow=0,temple_timer_divider=0;
legacy_u8 temple_timer_initialized=FALSE;
legacy_u8 temple_timer_suspended=FALSE;
legacy_u8 temple_timer_dispatching=FALSE;

legacy_void TempleTimerPump()
{
  legacy_u64 now;
  legacy_u16 index;
  legacy_u8 *pending[TEMPLE_TIMER_CALLBACK_COUNT];
  if(!temple_timer_initialized||temple_timer_dispatching) {
    return;
  }
  temple_timer_dispatching=TRUE;
  now=ToI64(tS*TEMPLE_TIMER_TICKS_PER_SECOND);
  // The PIT-backed clock can briefly read backward around a hardware reload.
  // Keep the last consumed tick so recovery cannot dispatch the same time twice.
  while(temple_timer_last_tick<now) {
    temple_timer_last_tick++;
    temple_timer_realtime++;
    temple_timer_divider++;
    if(temple_timer_divider==TEMPLE_TIMER_SLOW_DIVIDER) {
      temple_timer_divider=0;
      temple_timer_slow++;
    }
    if(!temple_timer_suspended) {
      temple_timer_counter++;
      MemCpy(pending,temple_timer_callbacks,sizeof(pending));
      for(index=0;index<TEMPLE_TIMER_CALLBACK_COUNT;index++) {
        if(pending[index]) {
          Call(pending[index]);
        }
      }
      TempleAudioTick();
    }
  }
  temple_timer_dispatching=FALSE;
}

legacy_s16 RsDosTimerRegisterCallback(legacy_u8 *callback)
{
  legacy_u16 index;
  for(index=0;index<TEMPLE_TIMER_CALLBACK_COUNT;index++) {
    if(!temple_timer_callbacks[index]) {
      temple_timer_callbacks[index]=callback;
      return TRUE;
    }
  }
  return FALSE;
}

legacy_void RsDosTimerUnregisterCallback(legacy_u8 *callback)
{
  legacy_u16 index;
  for(index=0;index<TEMPLE_TIMER_CALLBACK_COUNT;index++) {
    if(temple_timer_callbacks[index]==callback) {
      RsMemmove(temple_timer_callbacks+index,temple_timer_callbacks+index+1,
          (TEMPLE_TIMER_CALLBACK_COUNT-index-1)*sizeof(legacy_u8 *));
      temple_timer_callbacks[TEMPLE_TIMER_CALLBACK_COUNT-1]=NULL;
      return;
    }
  }
}

legacy_void RsDosTimerSetupInterrupt()
{
  if(temple_timer_initialized) {
    return;
  }
  temple_timer_counter=0;
  temple_timer_realtime=0;
  temple_timer_previous=0;
  temple_timer_slow=0;
  temple_timer_divider=0;
  temple_timer_suspended=FALSE;
  temple_timer_dispatching=FALSE;
  MemSet(temple_timer_callbacks,0,sizeof(temple_timer_callbacks));
  temple_timer_last_tick=ToI64(tS*TEMPLE_TIMER_TICKS_PER_SECOND);
  temple_timer_initialized=TRUE;
}

legacy_void RsDosTimerShutdown()
{
  temple_timer_initialized=FALSE;
  MemSet(temple_timer_callbacks,0,sizeof(temple_timer_callbacks));
}

legacy_void RsDosTimerResetCounter()
{
  TempleTimerPump();
  temple_timer_counter=0;
  temple_timer_previous=0;
}

legacy_void RsDosTimerSetCallbacksSuspended(legacy_s16 value)
{
  TempleTimerPump();
  temple_timer_suspended=(value&TEMPLE_BYTE_MASK)!=0;
}

legacy_u32 RsDosTimerGetRealtimeCounter()
{
  TemplePump();
  return temple_timer_realtime;
}

legacy_u32 RsTimerGetCounter()
{
  legacy_u32 previous=temple_timer_counter;
  TemplePump();
  if(!temple_timer_dispatching&&temple_timer_initialized&&previous==temple_timer_counter) {
    Sleep(TEMPLE_TIMER_POLL_MS);
    TemplePump();
  }
  return temple_timer_counter;
}

legacy_u32 RsTimerGetDelta()
{
  legacy_u32 current=RsTimerGetCounter();
  legacy_u32 delta=current-temple_timer_previous;
  temple_timer_previous=current;
  return delta;
}

legacy_u32 RsTimerGetSlowCounter()
{
  RsTimerGetCounter();
  return temple_timer_slow;
}
