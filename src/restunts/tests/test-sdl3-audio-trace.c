/* Standalone trace-format and I/O failure tests; no SDL or audio device needed. */
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../c/legacy.h"

static const legacy_char *trace_path;
static legacy_u32 open_count;
static legacy_u32 close_count;
static legacy_u32 flush_count;
static legacy_u32 error_count;
static legacy_s32 fail_open;
static legacy_s32 fail_write;
static legacy_s32 fail_flush;
static legacy_s32 fail_close;
static legacy_char captured_trace[2048];

static legacy_char *trace_test_getenv(const legacy_char *name)
{
	assert(strcmp(name, "RESTUNTS_AUDIO_TRACE") == 0);
	return (legacy_char *)trace_path;
}

static FILE *trace_test_fopen(const legacy_char *path, const legacy_char *mode)
{
	assert(strcmp(path, "test.trace") == 0);
	assert(strcmp(mode, "wb") == 0);
	++open_count;
	if (fail_open) {
		return NULL;
	}
	FILE *file = tmpfile();
	assert(file != NULL);
	return file;
}

/* These wrappers preserve the native stdio return types. */
static legacy_int trace_test_fprintf(FILE *file, const legacy_char *format, ...)
{
	if (file == stderr) {
		++error_count;
		return 0;
	}
	if (fail_write) {
		return -1;
	}
	va_list arguments;
	va_start(arguments, format);
	legacy_int result = vfprintf(file, format, arguments);
	va_end(arguments);
	return result;
}

static legacy_int trace_test_fflush(FILE *file)
{
	++flush_count;
	return fail_flush ? EOF : fflush(file);
}

static legacy_int trace_test_fclose(FILE *file)
{
	++close_count;
	assert(fflush(file) == 0);
	rewind(file);
	size_t size = fread(captured_trace, 1, sizeof(captured_trace) - 1, file);
	assert(feof(file));
	captured_trace[size] = '\0';
	assert(fclose(file) == 0);
	return fail_close ? EOF : 0;
}

#define getenv trace_test_getenv
#define fopen trace_test_fopen
#define fprintf trace_test_fprintf
#define fflush trace_test_fflush
#define fclose trace_test_fclose
#include "../platform/sdl3/audio_trace.h"
#undef getenv
#undef fopen
#undef fprintf
#undef fflush
#undef fclose

static void test_disabled(void)
{
	adlib_trace_open("nuked", 3579545, 44100);
	adlib_trace_write(0x20, 0xFF, 0);
	adlib_trace_advance(441);
	adlib_trace_close();
	trace_path = "";
	adlib_trace_open("nuked", 3579545, 44100);
	adlib_trace_close();
	assert(open_count == 0 && close_count == 0 && flush_count == 0 && error_count == 0);
}

static void test_order_and_timestamps(void)
{
	trace_path = "test.trace";
	adlib_trace_open("nuked", 3579545, 44100);
	assert(flush_count == 1);
	adlib_trace_write(0x20, 0xFF, 0);
	adlib_trace_write(0xB0, 0x20, 1);
	adlib_trace_write(0xB0, 0, 0);
	adlib_trace_advance(441);
	adlib_trace_write(0xB0, 0x20, 1);
	adlib_trace_clear();
	adlib_trace_advance(44100 - 442);
	assert(flush_count == 1);
	adlib_trace_advance(1);
	assert(flush_count == 2);
	adlib_trace_write(0xA0, 0x7F, 0);
	adlib_trace_advance(441);
	adlib_trace_close();
	adlib_trace_close();
	assert(close_count == 1);
	assert(strcmp(captured_trace, "RESTUNTS_OPL_TRACE 1 nuked 3579545 44100\n"
								  "0 W 20 FF 0\n"
								  "0 W B0 20 1\n"
								  "0 W B0 00 0\n"
								  "441 W B0 20 1\n"
								  "441 C\n"
								  "44100 W A0 7F 0\n"
								  "44541 E\n") == 0);

	adlib_trace_open("nuked", 3579545, 44100);
	adlib_trace_advance(UINT32_MAX);
	adlib_trace_advance(2);
	adlib_trace_write(0x01, 0x20, 1);
	adlib_trace_open("nuked", 3579545, 44100);
	assert(strcmp(captured_trace, "RESTUNTS_OPL_TRACE 1 nuked 3579545 44100\n"
								  "4294967297 W 01 20 1\n"
								  "4294967297 E\n") == 0);
	adlib_trace_close();
	assert(strcmp(captured_trace, "RESTUNTS_OPL_TRACE 1 nuked 3579545 44100\n0 E\n") == 0);
}

static void test_full_width_fields(void)
{
	adlib_trace_open("nuked", LEGACY_U32_MAX, LEGACY_U32_MAX);
	adlib_trace_frames = ~(legacy_u64)0;
	adlib_trace_write(LEGACY_U32_MAX, LEGACY_U32_MAX, -1);
	adlib_trace_clear();
	adlib_trace_close();
	assert(strcmp(captured_trace, "RESTUNTS_OPL_TRACE 1 nuked 4294967295 4294967295\n"
								  "18446744073709551615 W FFFFFFFF FFFFFFFF 1\n"
								  "18446744073709551615 C\n"
								  "18446744073709551615 E\n") == 0);
}

static void assert_failed_trace_is_disabled(void)
{
	assert(adlib_trace_file == NULL);
	assert(error_count == 1);
	legacy_u32 closes = close_count;
	legacy_u32 flushes = flush_count;
	adlib_trace_write(0x20, 0xFF, 0);
	adlib_trace_clear();
	adlib_trace_advance(44100);
	adlib_trace_close();
	assert(close_count == closes && flush_count == flushes);
}

static void test_io_failures(void)
{
	fail_open = 1;
	adlib_trace_open("nuked", 3579545, 44100);
	assert_failed_trace_is_disabled();
	fail_open = 0;

	fail_write = 1;
	adlib_trace_open("nuked", 3579545, 44100);
	assert_failed_trace_is_disabled();
	fail_write = 0;

	adlib_trace_open("nuked", 3579545, 44100);
	fail_write = 1;
	adlib_trace_write(0x20, 0xFF, 0);
	assert_failed_trace_is_disabled();
	fail_write = 0;

	adlib_trace_open("nuked", 3579545, 44100);
	fail_write = 1;
	adlib_trace_clear();
	assert_failed_trace_is_disabled();
	fail_write = 0;

	fail_flush = 1;
	adlib_trace_open("nuked", 3579545, 44100);
	assert_failed_trace_is_disabled();
	fail_flush = 0;

	adlib_trace_open("nuked", 3579545, 44100);
	fail_flush = 1;
	adlib_trace_advance(44100);
	assert_failed_trace_is_disabled();
	fail_flush = 0;

	adlib_trace_open("nuked", 3579545, 44100);
	fail_write = 1;
	adlib_trace_close();
	assert_failed_trace_is_disabled();
	fail_write = 0;

	adlib_trace_open("nuked", 3579545, 44100);
	fail_close = 1;
	adlib_trace_close();
	assert_failed_trace_is_disabled();
	fail_close = 0;
	assert(open_count == close_count + 1);
}

legacy_int main(void)
{
	test_disabled();
	test_order_and_timestamps();
	test_full_width_fields();
	test_io_failures();
	puts("SDL3 audio trace tests passed");
	return 0;
}
