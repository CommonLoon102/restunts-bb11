// Run only in a fresh disposable VM task: replaces clock and sleep for this test.
#include "LEGACY.HH"
#include "LIBC.HC"
#include "NATIVE.HH"

#define TEMPLE_TIMER_TEST_START_SECONDS 10.0
#define TEMPLE_TIMER_TEST_TICK_FRACTION 0.25
#define TEMPLE_TIMER_TEST_TICKS_PER_SECOND 100
#define TEMPLE_TIMER_TEST_SLOW_TICKS_PER_SECOND 20
#define TEMPLE_TIMER_TEST_POLL_ROUNDS 32
#define TEMPLE_TIMER_TEST_DISPATCH_LIMIT (TEMPLE_TIMER_TEST_TICKS_PER_SECOND*4)
#define TEMPLE_TIMER_TEST_INITIAL_TICKS 3
#define TEMPLE_TIMER_TEST_BACKWARD_TICKS 2
#define TEMPLE_TIMER_TEST_FORWARD_TICKS 8
#define TEMPLE_TIMER_TEST_SUSPEND_TICKS 13
#define TEMPLE_TIMER_TEST_RESUME_TICKS 18
#define TEMPLE_TIMER_TEST_RESET_PROGRESS_TICKS 23

legacy_f64 temple_timer_test_time=TEMPLE_TIMER_TEST_START_SECONDS;
legacy_u32 temple_timer_test_callbacks=0,temple_timer_test_audio=0;

legacy_void TempleTimerTestAssert(legacy_u8 condition,legacy_u8 *message)
{
  if(!condition) {
    PrintErr("Timer test: %s\n",message);
    throw('Timer');
  }
}

legacy_void TempleTimerTestSleep(legacy_s64 milliseconds)
{
  no_warn milliseconds;
}

// HolyC has no #undef; these replacements stay in this disposable test task.
#define tS temple_timer_test_time
#define Sleep TempleTimerTestSleep
#include "TIMER.HC"

legacy_void TempleAudioTick()
{
  temple_timer_test_audio++;
  TempleTimerTestAssert(
      temple_timer_test_audio<=TEMPLE_TIMER_TEST_DISPATCH_LIMIT,"bounded audio dispatch");
}

legacy_void TemplePump()
{
  TempleTimerPump();
}

legacy_void TempleTimerTestCallback()
{
  temple_timer_test_callbacks++;
}

legacy_void TempleTimerTestReset()
{
  RsDosTimerShutdown();
  temple_timer_test_time=TEMPLE_TIMER_TEST_START_SECONDS;
  temple_timer_test_callbacks=0;
  temple_timer_test_audio=0;
  RsDosTimerSetupInterrupt();
  TempleTimerTestAssert(
      RsDosTimerRegisterCallback(&TempleTimerTestCallback),"register timer callback");
}

legacy_void TempleTimerTestSample(legacy_u32 ticks)
{
  temple_timer_test_time=TEMPLE_TIMER_TEST_START_SECONDS+
      (ToF64(ticks)+TEMPLE_TIMER_TEST_TICK_FRACTION)/TEMPLE_TIMER_TEST_TICKS_PER_SECOND;
  TempleTimerPump();
}

legacy_void TempleTimerTestCounters(
    legacy_u32 game,legacy_u32 realtime,legacy_u32 callbacks,legacy_u32 audio)
{
  TempleTimerTestAssert(RsTimerGetCounter()==game,"game counter matches elapsed time");
  TempleTimerTestAssert(
      RsDosTimerGetRealtimeCounter()==realtime,"realtime counter matches elapsed time");
  TempleTimerTestAssert(RsTimerGetSlowCounter()==
          realtime*TEMPLE_TIMER_TEST_SLOW_TICKS_PER_SECOND/TEMPLE_TIMER_TEST_TICKS_PER_SECOND,
      "slow counter keeps its 20 Hz cadence");
  TempleTimerTestAssert(
      temple_timer_test_callbacks==callbacks,"callbacks run once per active tick");
  TempleTimerTestAssert(temple_timer_test_audio==audio,"audio runs once per active tick");
}

legacy_void TempleTimerTests()
{
  legacy_u16 noreg round;
  legacy_u32 noreg interval=TEMPLE_TIMER_TEST_SUSPEND_TICKS-TEMPLE_TIMER_TEST_FORWARD_TICKS;
  TempleTimerTestReset();
  TempleTimerTestCounters(0,0,0,0);
  TempleTimerTestSample(TEMPLE_TIMER_TEST_TICKS_PER_SECOND);
  for(round=0;round<TEMPLE_TIMER_TEST_POLL_ROUNDS;round++) {
    TempleTimerTestCounters(TEMPLE_TIMER_TEST_TICKS_PER_SECOND,TEMPLE_TIMER_TEST_TICKS_PER_SECOND,
        TEMPLE_TIMER_TEST_TICKS_PER_SECOND,TEMPLE_TIMER_TEST_TICKS_PER_SECOND);
  }
  TempleTimerTestAssert(RsTimerGetDelta()==TEMPLE_TIMER_TEST_TICKS_PER_SECOND,
      "one second produces 100 delta ticks");
  TempleTimerTestAssert(RsTimerGetDelta()==0,"repeated delta read consumes no time");

  TempleTimerTestReset();
  TempleTimerTestSample(TEMPLE_TIMER_TEST_INITIAL_TICKS);
  for(round=0;round<TEMPLE_TIMER_TEST_POLL_ROUNDS;round++) {
    TempleTimerTestSample(TEMPLE_TIMER_TEST_BACKWARD_TICKS);
    TempleTimerTestSample(TEMPLE_TIMER_TEST_INITIAL_TICKS);
    TempleTimerTestAssert(temple_timer_test_callbacks==TEMPLE_TIMER_TEST_INITIAL_TICKS,
        "clock rollback and rebound must not replay consumed ticks");
  }
  TempleTimerTestSample(TEMPLE_TIMER_TEST_FORWARD_TICKS);
  TempleTimerTestCounters(TEMPLE_TIMER_TEST_FORWARD_TICKS,TEMPLE_TIMER_TEST_FORWARD_TICKS,
      TEMPLE_TIMER_TEST_FORWARD_TICKS,TEMPLE_TIMER_TEST_FORWARD_TICKS);
  RsDosTimerSetCallbacksSuspended(TRUE);
  TempleTimerTestSample(TEMPLE_TIMER_TEST_SUSPEND_TICKS);
  TempleTimerTestSample(TEMPLE_TIMER_TEST_BACKWARD_TICKS);
  TempleTimerTestSample(TEMPLE_TIMER_TEST_SUSPEND_TICKS);
  TempleTimerTestCounters(TEMPLE_TIMER_TEST_FORWARD_TICKS,TEMPLE_TIMER_TEST_SUSPEND_TICKS,
      TEMPLE_TIMER_TEST_FORWARD_TICKS,TEMPLE_TIMER_TEST_FORWARD_TICKS);
  RsDosTimerSetCallbacksSuspended(FALSE);
  TempleTimerTestSample(TEMPLE_TIMER_TEST_RESUME_TICKS);
  TempleTimerTestCounters(TEMPLE_TIMER_TEST_SUSPEND_TICKS,TEMPLE_TIMER_TEST_RESUME_TICKS,
      TEMPLE_TIMER_TEST_SUSPEND_TICKS,TEMPLE_TIMER_TEST_SUSPEND_TICKS);

  RsDosTimerResetCounter();
  TempleTimerTestAssert(RsTimerGetDelta()==0,"counter reset also resets the delta baseline");
  RsDosTimerSetupInterrupt();
  TempleTimerTestCounters(0,TEMPLE_TIMER_TEST_RESUME_TICKS,TEMPLE_TIMER_TEST_SUSPEND_TICKS,
      TEMPLE_TIMER_TEST_SUSPEND_TICKS);
  TempleTimerTestSample(TEMPLE_TIMER_TEST_RESET_PROGRESS_TICKS);
  TempleTimerTestCounters(interval,TEMPLE_TIMER_TEST_RESET_PROGRESS_TICKS,
      TEMPLE_TIMER_TEST_RESUME_TICKS,TEMPLE_TIMER_TEST_RESUME_TICKS);
  TempleTimerTestAssert(RsTimerGetDelta()==interval,"counter reset preserves clock cadence");
  RsDosTimerShutdown();
  Print("Deterministic timer tests passed.\n");
}

TempleTimerTests();
