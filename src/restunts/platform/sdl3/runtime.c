#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* The shared runtime also serves tools and tests without SDL dependencies. */
#ifdef RESTUNTS_FULL
#include "sdl3.h"
#endif
#if defined(RESTUNTS_FULL) && !defined(__EMSCRIPTEN__) && !defined(__DJGPP__)
#include <SDL3/SDL_messagebox.h>
#endif
#include "../../c/platform.h"

legacy_s32 sdl3_batch_mode;

void *_memcpy(void *destination, const void *source, legacy_u16 length)
{
	return memcpy(destination, source, length);
}

void *__fmemcpy(void *destination, const void *source, legacy_u16 length)
{
	return memcpy(destination, source, length);
}

legacy_s16 dos_write_stdout(const legacy_s8 *text, legacy_u16 length)
{
	return fwrite(text, 1, length, stdout) == length ? (legacy_s16)length : -1;
}

legacy_s16 dos_write_stderr(const legacy_s8 *text, legacy_u16 length)
{
	return fwrite(text, 1, length, stderr) == length ? (legacy_s16)length : -1;
}

void dos_show_startup_error(const legacy_s8 *message)
{
	dos_write_stderr(message, (legacy_u16)strlen((const legacy_char *)message));
#if defined(RESTUNTS_FULL) && !defined(__EMSCRIPTEN__) && !defined(__DJGPP__)
	if (!sdl3_batch_mode) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Restunts", (const legacy_char *)message,
								 NULL);
	}
#endif
}

void dos_process_exit(legacy_s16 status)
{
#ifdef RESTUNTS_FULL
	/* Fatal exits need the same driver lifetime ordering as returning from main. */
	sdl3_platform_shutdown();
#endif
	exit(status);
}

void headless_exit(legacy_s16 status)
{
	dos_process_exit(status);
}

legacy_s16 dos_data_stack_segments_match(void)
{
	return 1;
}

void dos_install_divide_error_handler(void)
{
	/* Portable arithmetic handles the original divide recovery explicitly. */
}

void dos_interrupts_disable(void)
{
	/* Native game callbacks run synchronously on the main thread. */
}

void dos_interrupts_enable(void)
{
}

void dos_set_critical_error_handler(legacy_s16 (*callback)(void))
{
	(void)callback;
}
