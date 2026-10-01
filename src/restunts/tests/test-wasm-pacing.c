#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>
#include <emscripten.h>
#include "../platform/sdl3/sdl3.h"

enum {
	TEST_START_MS = 1000,
	TEST_IDLE_INTERVAL_MS = 8,
	TEST_DELAY_MS = 1,
	TEST_SEQUENTIAL_WAITS = 32,
	TEST_NS_PER_MS = 1000000
};

static legacy_u64 mock_time_ms = TEST_START_MS;

static legacy_u64 test_get_ticks(void)
{
	return mock_time_ms;
}

static legacy_u64 test_get_ticks_ns(void)
{
	return mock_time_ms * TEST_NS_PER_MS;
}

/* Keep the real Asyncify/MessageChannel implementation, controlling only the
 * clock used to decide whether input polling needs another browser yield. */
#define SDL_GetTicks test_get_ticks
#define SDL_GetTicksNS test_get_ticks_ns
#include "../platform/sdl3/timer.c"
#undef SDL_GetTicks
#undef SDL_GetTicksNS

void sdl3_platform_pump(void)
{
	sdl3_timer_pump();
}

void sdl3_audio_update(void)
{
}

EM_JS(void, test_pacing_begin, (), { globalThis.resetPacingObservation(); });

EM_JS(legacy_u32, test_resume_count, (), { return globalThis.pacingObservation.resumes; });

EM_JS(void, test_pacing_finish, (legacy_u32 expected_resumes, legacy_u32 expected_timers), {
	globalThis.pacingObservation.expectedResumes = expected_resumes;
	globalThis.pacingObservation.expectedTimers = expected_timers;
	globalThis.pacingObservation.finished = true;
});

legacy_int main(void)
{
	test_pacing_begin();
	sdl3_platform_delay(0);
	legacy_u32 expected_resumes = 1;
	assert(test_resume_count() == expected_resumes);

	/* Explicit delays and input-only polling share the same last-yield time. */
	sdl3_browser_yield_if_due();
	assert(test_resume_count() == expected_resumes);
	mock_time_ms += TEST_IDLE_INTERVAL_MS - 1U;
	sdl3_browser_yield_if_due();
	assert(test_resume_count() == expected_resumes);
	mock_time_ms++;
	sdl3_browser_yield_if_due();
	assert(test_resume_count() == ++expected_resumes);

	mock_time_ms += TEST_IDLE_INTERVAL_MS;
	sdl3_platform_delay(TEST_DELAY_MS);
	assert(test_resume_count() == ++expected_resumes);
	sdl3_browser_yield_if_due();
	assert(test_resume_count() == expected_resumes);

	/* More than five sequential short waits exercise the browser timer nesting
	 * rule. The driver checks that each continuation runs in a message task. */
	for (legacy_u32 index = 0; index < TEST_SEQUENTIAL_WAITS; index++) {
		sdl3_platform_delay(TEST_DELAY_MS);
		assert(test_resume_count() == ++expected_resumes);
		sdl3_browser_yield_if_due();
		assert(test_resume_count() == expected_resumes);
	}
	test_pacing_finish(expected_resumes, TEST_SEQUENTIAL_WAITS + 1U);
	puts("WebAssembly pacing regression tests passed");
	return 0;
}
