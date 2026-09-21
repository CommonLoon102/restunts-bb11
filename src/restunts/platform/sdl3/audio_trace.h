#ifndef RESTUNTS_SDL3_AUDIO_TRACE_H
#define RESTUNTS_SDL3_AUDIO_TRACE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Main-thread diagnostic only. An unset or empty path leaves synthesis alone.
 * Each open truncates the selected file and starts at rendered PCM frame zero.
 * Timestamps describe generated PCM, not when SDL plays its queued samples. */
static FILE *adlib_trace_file;
static uint64_t adlib_trace_frames;
static uint64_t adlib_trace_last_flush;
static unsigned int adlib_trace_sample_rate;
static int adlib_trace_error_reported;

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
	if (fprintf(adlib_trace_file, "%llu E\n", (unsigned long long)adlib_trace_frames) < 0) {
		adlib_trace_fail();
		return;
	}
	FILE *file = adlib_trace_file;
	adlib_trace_file = NULL;
	if (fclose(file) != 0) {
		adlib_trace_fail();
	}
}

static void adlib_trace_open(const char *backend, unsigned int clock, unsigned int rate)
{
	adlib_trace_close();
	adlib_trace_frames = 0;
	adlib_trace_last_flush = 0;
	adlib_trace_sample_rate = rate;
	const char *path = getenv("RESTUNTS_AUDIO_TRACE");
	if (path == NULL || path[0] == '\0') {
		return;
	}
	adlib_trace_file = fopen(path, "wb");
	if (adlib_trace_file == NULL) {
		adlib_trace_fail();
		return;
	}
	if (fprintf(adlib_trace_file, "RESTUNTS_OPL_TRACE 1 %s %u %u\n", backend, clock, rate) < 0 ||
		fflush(adlib_trace_file) != 0) {
		adlib_trace_fail();
	}
}

static void adlib_trace_write(unsigned int reg, unsigned int value, int buffered)
{
	if (adlib_trace_file != NULL &&
		fprintf(adlib_trace_file, "%llu W %02X %02X %d\n", (unsigned long long)adlib_trace_frames,
				reg, value, buffered != 0) < 0) {
		adlib_trace_fail();
	}
}

static void adlib_trace_clear(void)
{
	if (adlib_trace_file != NULL &&
		fprintf(adlib_trace_file, "%llu C\n", (unsigned long long)adlib_trace_frames) < 0) {
		adlib_trace_fail();
	}
}

static void adlib_trace_advance(unsigned int frames)
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
