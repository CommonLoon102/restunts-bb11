#ifndef RESTUNTS_COMMAND_LINE_H
#define RESTUNTS_COMMAND_LINE_H

#include "legacy.h"

enum COMMAND_LINE_RESULT {
	COMMAND_LINE_CONTINUE = -1,
	COMMAND_LINE_SUCCESS = 0,
	COMMAND_LINE_FAILURE = 1
};

/* Print the first help/version request without initializing the game. */
legacy_s16 command_line_print_info(legacy_s32 argc, legacy_s8 *argv[]);

#endif
