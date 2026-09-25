#ifndef RESTUNTS_SDL3_AUDIO_TRACE_H
#define RESTUNTS_SDL3_AUDIO_TRACE_H

#include "../../c/legacy.h"
#include <stdio.h>
#include <stdlib.h>

/* Main-thread diagnostic only. An unset or empty path leaves synthesis alone.
 * Each open truncates the selected file and starts at rendered PCM frame zero.
 * Timestamps describe generated PCM, not when SDL plays its queued samples.
 * Legacy printf fragments match each argument on every supported compiler. */
static FILE *adlib_trace_file;
static legacy_u64 adlib_trace_frames;
static legacy_u64 adlib_trace_last_flush;
static legacy_u32 adlib_trace_sample_rate;
static legacy_s32 adlib_trace_error_reported;

static void adlib_trace_fail(void)
{
	FILE *file = adlib_trace_file;
	adlib_trace_file = NULL;
	if (file != NULL) {
		fclose(file);
	}
	if (!adlib_trace_error_reported) {
		fprintf(stderr, "Audio trace disabled after an I/O error.\n");
		adlib_trace_error_reported = 1;
	}
}

static void adlib_trace_close(void)
{
	if (adlib_trace_file == NULL) {
		return;
	}
	if (fprintf(adlib_trace_file, "%" LEGACY_PRIu64 " E\n", adlib_trace_frames) < 0) {
		adlib_trace_fail();
		return;
	}
	FILE *file = adlib_trace_file;
	adlib_trace_file = NULL;
	if (fclose(file) != 0) {
		adlib_trace_fail();
	}
}

static void adlib_trace_open(const legacy_char *backend, legacy_u32 clock, legacy_u32 rate)
{
	adlib_trace_close();
	adlib_trace_frames = 0;
	adlib_trace_last_flush = 0;
	adlib_trace_sample_rate = rate;
	const legacy_char *path = getenv("RESTUNTS_AUDIO_TRACE");
	if (path == NULL || path[0] == '\0') {
		return;
	}
	adlib_trace_file = fopen(path, "wb");
	if (adlib_trace_file == NULL) {
		adlib_trace_fail();
		return;
	}
	if (fprintf(adlib_trace_file, "RESTUNTS_OPL_TRACE 1 %s %" LEGACY_PRIu32 " %" LEGACY_PRIu32 "\n",
				backend, clock, rate) < 0 ||
		fflush(adlib_trace_file) != 0) {
		adlib_trace_fail();
	}
}

static void adlib_trace_write(legacy_u32 reg, legacy_u32 value, legacy_s32 buffered)
{
	if (adlib_trace_file != NULL &&
		fprintf(adlib_trace_file,
				"%" LEGACY_PRIu64 " W %02" LEGACY_PRIX32 " %02" LEGACY_PRIX32 " %d\n",
				adlib_trace_frames, reg, value, buffered != 0) < 0) {
		adlib_trace_fail();
	}
}

static void adlib_trace_clear(void)
{
	if (adlib_trace_file != NULL &&
		fprintf(adlib_trace_file, "%" LEGACY_PRIu64 " C\n", adlib_trace_frames) < 0) {
		adlib_trace_fail();
	}
}

static void adlib_trace_advance(legacy_u32 frames)
{
	if (adlib_trace_file == NULL) {
		return;
	}
	adlib_trace_frames += frames;
	if (adlib_trace_frames - adlib_trace_last_flush >= adlib_trace_sample_rate) {
		if (fflush(adlib_trace_file) != 0) {
			adlib_trace_fail();
			return;
		}
		adlib_trace_last_flush = adlib_trace_frames;
	}
}

#endif
