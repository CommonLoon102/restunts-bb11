#include "sdl3.h"
#include "../../c/platform.h"
#include "../../c/presentation.h"
#include <string.h>

#define TIMER_CALLBACK_CAPACITY DOS_TIMER_USABLE_CALLBACK_COUNT
#define TIMER_TICK_MS 10U
#define TIMER_POLL_DELAY_MS 1U

static void (*callbacks[TIMER_CALLBACK_CAPACITY])(void);
static legacy_u64 last_tick;
static legacy_u32 game_counter;
static legacy_u32 realtime_counter;
static legacy_u32 last_counter;
static legacy_u32 slow_counter;
static legacy_u32 slow_divider;
static legacy_u8 initialized;
static legacy_u8 suspended;
static legacy_u8 dispatching;

legacy_u64 presentation_now(void)
{
	return SDL_GetTicksNS();
}

void sdl3_timer_pump(void)
{
	if (!initialized || dispatching) {
		return;
	}
	dispatching = true;
	legacy_u64 now = SDL_GetTicks();
	while (now - last_tick >= TIMER_TICK_MS) {
		last_tick += TIMER_TICK_MS;
		realtime_counter++;
		if (++slow_divider == DOS_TIMER_DEFAULT_DIVIDER_PERIOD) {
			slow_divider = 0;
			slow_counter++;
		}
		if (!suspended) {
			game_counter++;
			/* Snapshot: callbacks can unregister themselves during dispatch. */
			void (*pending[TIMER_CALLBACK_CAPACITY])(void);
			memcpy(pending, callbacks, sizeof(pending));
			for (legacy_u32 index = 0; index < TIMER_CALLBACK_CAPACITY; index++) {
				if (pending[index] != NULL) {
					pending[index]();
				}
			}
		}
		sdl3_audio_update();
	}
	dispatching = false;
}

legacy_s16 dos_timer_register_callback(void (*callback)(void))
{
	for (legacy_u32 index = 0; index < TIMER_CALLBACK_CAPACITY; index++) {
		if (callbacks[index] == NULL) {
			callbacks[index] = callback;
			return DOS_TIMER_CALLBACK_REGISTRATION_SUCCEEDED;
		}
	}
	return DOS_TIMER_CALLBACK_REGISTRATION_FAILED;
}

void dos_timer_unregister_callback(void (*callback)(void))
{
	for (legacy_u32 index = 0; index < TIMER_CALLBACK_CAPACITY; index++) {
		if (callbacks[index] == callback) {
			memmove(callbacks + index, callbacks + index + 1U,
					(TIMER_CALLBACK_CAPACITY - index - 1U) * sizeof(callbacks[0]));
			callbacks[TIMER_CALLBACK_CAPACITY - 1U] = NULL;
			return;
		}
	}
}

void dos_timer_setup_interrupt(void)
{
	if (initialized) {
		return;
	}
	initialized = true;
	last_tick = SDL_GetTicks();
	game_counter = 0;
	realtime_counter = 0;
	last_counter = 0;
	slow_counter = 0;
	slow_divider = 0;
	suspended = false;
	memset(callbacks, 0, sizeof(callbacks));
}

void dos_timer_shutdown(void)
{
	initialized = false;
	memset(callbacks, 0, sizeof(callbacks));
}

void dos_timer_reset_counter(void)
{
	sdl3_timer_pump();
	game_counter = 0;
	last_counter = 0;
}

void dos_timer_set_callbacks_suspended(legacy_s16 value)
{
	sdl3_timer_pump();
	suspended = (value & DOS_TIMER_CALLBACK_SUSPENDED_MASK) != 0;
}

legacy_u32 dos_timer_get_realtime_counter(void)
{
	sdl3_platform_pump();
	return realtime_counter;
}

legacy_u32 timer_get_counter(void)
{
	static legacy_u32 previous_read;
	sdl3_platform_pump();
	/* Legacy waits poll this accessor. Yield without delaying timer callbacks
	 * or consuming an entire 10ms tick, so menus and frame pacing stay responsive. */
	if (!dispatching && initialized && previous_read == game_counter) {
		SDL_Delay(TIMER_POLL_DELAY_MS);
		sdl3_platform_pump();
	}
	previous_read = game_counter;
	return game_counter;
}

legacy_u32 timer_get_delta(void)
{
	legacy_u32 current = timer_get_counter();
	legacy_u32 delta = current - last_counter;
	last_counter = current;
	return delta;
}

legacy_u32 timer_get_slow_counter(void)
{
	(void)timer_get_counter();
	return slow_counter;
}
