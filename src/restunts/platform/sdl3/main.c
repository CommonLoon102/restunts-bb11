#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "../../c/restunts.h"
#include "sdl3.h"

extern void full_data_initialize(void);
extern legacy_s16 stuntsmain(legacy_s16 argc, legacy_s8 *argv[]);

/* The C runtime requires native int parameters and return type. */
int main(int argc, char **argv)
{
#if !defined(__DJGPP__) && !defined(RESTUNTS_HEADLESS)
	if (argc == 2 && strcmp(argv[1], "--licenses") == 0) {
		fputs("Nuked OPL2 Lite - Copyright (C) 2026 Nuke.YKT.\n"
			  "GNU LGPL version 2.1 or, at your option, any later version.\n"
			  "Provided WITHOUT ANY WARRANTY. See the complete license at\n"
			  "share/licenses/restunts/Nuked-OPL2-LICENSE in the package,\n"
			  "or third_party/nuked-opl2-lite/LICENSE in the source checkout.\n"
			  "Library source and rebuild instructions: share/restunts/nuked-opl2-lite/\n"
			  "You may modify this application for your own use and reverse engineer it\n"
			  "to debug modifications to this LGPL-covered library.\n"
			  "See THIRD-PARTY-NOTICES.txt for dependency notices.\n",
			  stdout);
		return 0;
	}
#endif
#if defined(RESTUNTS_HEADLESS) || defined(RESTUNTS_PIXLDUMP)
	sdl3_batch_mode = 1;
#endif
	/* Accept a resource directory without changing the historical game and
 * dump argument syntax. By default resources are read from the current dir. */
	if (argc >= 2 && strcmp(argv[1], "--data-dir") == 0) {
		if (argc < 3 || chdir(argv[2]) != 0) {
			fputs("Cannot open --data-dir directory\n", stderr);
			return 1;
		}
		for (legacy_s32 index = 1; index + 2 <= argc; index++) {
			argv[index] = argv[index + 2];
		}
		argc -= 2;
	}
	if (argc > 32767) {
		fputs("Too many arguments\n", stderr);
		return 1;
	}
	if (!SDL_Init(0)) {
		fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
		return 1;
	}
#ifdef RESTUNTS_HEADLESS
	atexit(SDL_Quit);
#else
	atexit(sdl3_platform_shutdown);
	full_data_initialize();
#endif
#if defined(RESTUNTS_HEADLESS) || defined(RESTUNTS_PIXLDUMP)
	return stuntsmain((legacy_s16)argc, (legacy_s8 **)argv);
#else
	return run_main_menu_loop((legacy_s16)argc, (legacy_s8 **)argv);
#endif
}
