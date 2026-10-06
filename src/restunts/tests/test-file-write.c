#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../c/fileio.h"
#include "../c/platform.h"
#include "../c/fatal.h"

#define TEST_FILE_HANDLE 7U
#define TEST_FILE_CAPACITY (LEGACY_U16_MAX + 2U)
#define TEST_SHORT_WRITE_AFTER 17000U
#define TEST_PATTERN_MULTIPLIER 37U
#define TEST_SMALL_FILE_SIZE 13U

static const legacy_s8 test_track_path[] = "Tracks\\MyTrack.TrK";
static const legacy_s8 test_score_path[] = "Tracks/MyTrack.HiG";
static const legacy_s8 test_replay_path[] = "Replay/MyReplay.RpL";
static legacy_u8 payload[TEST_FILE_CAPACITY];
static legacy_u8 written[TEST_FILE_CAPACITY];
static const legacy_s8 *expected_path;
static legacy_u32 expected_length;
static legacy_u32 written_length;
static legacy_u16 open_calls;
static legacy_u16 write_calls;
static legacy_u16 close_calls;
static legacy_u16 remove_calls;
static legacy_u16 notifications;
static legacy_u16 fatal_errors;
static legacy_u8 file_open;
static legacy_u8 file_exists;
static legacy_u8 fail_open;
static legacy_u8 fail_write;
static legacy_u8 fail_close;

static void reset_test(const legacy_s8 *path, legacy_u32 length)
{
	expected_path = path;
	expected_length = length;
	written_length = 0;
	open_calls = 0;
	write_calls = 0;
	close_calls = 0;
	remove_calls = 0;
	notifications = 0;
	fatal_errors = 0;
	file_open = 0;
	file_exists = 0;
	fail_open = 0;
	fail_write = 0;
	fail_close = 0;
	memset(written, 0, sizeof(written));
}

legacy_u16 dos_file_open(const legacy_s8 *path, legacy_s16 create)
{
	assert(strcmp((const legacy_char *)path, (const legacy_char *)expected_path) == 0);
	assert(create == DOS_FILE_CREATE);
	assert(!file_open);
	open_calls++;
	if (fail_open) {
		return 0;
	}
	file_open = 1;
	file_exists = 1;
	return TEST_FILE_HANDLE;
}

legacy_u16 dos_file_write(legacy_u16 handle, const void *source, legacy_u16 length)
{
	assert(handle == TEST_FILE_HANDLE && file_open);
	assert(written_length + length <= sizeof(written));
	write_calls++;
	legacy_u16 count = length;
	if (fail_write && written_length + count > TEST_SHORT_WRITE_AFTER) {
		assert(written_length <= TEST_SHORT_WRITE_AFTER);
		count = (legacy_u16)(TEST_SHORT_WRITE_AFTER - written_length);
	}
	memcpy(written + written_length, source, count);
	written_length += count;
	return count;
}

legacy_s16 dos_file_close(legacy_u16 handle)
{
	assert(handle == TEST_FILE_HANDLE && file_open);
	close_calls++;
	file_open = 0;
	return fail_close ? -1 : 0;
}

legacy_s16 dos_file_error(void)
{
	return 0;
}

legacy_s16 dos_file_remove(const legacy_s8 *path)
{
	assert(strcmp((const legacy_char *)path, (const legacy_char *)expected_path) == 0);
	assert(!file_open);
	remove_calls++;
	file_exists = 0;
	return 0;
}

void android_saved_file_written(const legacy_s8 *path)
{
	assert(strcmp((const legacy_char *)path, (const legacy_char *)expected_path) == 0);
	assert(!file_open && file_exists && close_calls == 1 && !fail_close);
	assert(written_length == expected_length);
	assert(memcmp(written, payload, expected_length) == 0);
	notifications++;
	/* The real callback reports shared-storage failure separately. Its void
	 * result must never cause a completed local save to be removed. */
}

void fatal_error(const legacy_s8 *format, ...)
{
	assert(format != NULL);
	fatal_errors++;
}

static void test_successful_save(const legacy_s8 *path, legacy_u32 length)
{
	reset_test(path, length);
	assert(file_write_fatal(path, payload, length) == 0);
	assert(open_calls == 1 && close_calls == 1 && remove_calls == 0);
	assert(!file_open && file_exists && fatal_errors == 0);
	assert(written_length == length);
	assert(memcmp(written, payload, length) == 0);
	if (length > LEGACY_U16_MAX) {
		assert(write_calls > 1);
	}
	if (length == 0) {
		assert(write_calls == 0);
	}
#ifdef __ANDROID__
	assert(notifications == 1);
#else
	assert(notifications == 0);
#endif
}

static void test_failed_create(void)
{
	reset_test(test_track_path, TEST_SMALL_FILE_SIZE);
	fail_open = 1;
	assert(file_write_fatal(test_track_path, payload, expected_length) != 0);
	assert(open_calls == 1 && write_calls == 0 && close_calls == 0);
	assert(remove_calls == 1 && notifications == 0 && fatal_errors == 0);
	assert(!file_open && !file_exists);
}

static void test_short_write(legacy_u8 fatal_on_error)
{
	reset_test(test_replay_path, sizeof(payload));
	fail_write = 1;
	legacy_s16 result;
	if (fatal_on_error) {
		result = file_write_nofatal(test_replay_path, payload, expected_length);
	} else {
		result = file_write_fatal(test_replay_path, payload, expected_length);
	}
	assert(result != 0);
	assert(written_length == TEST_SHORT_WRITE_AFTER && write_calls > 1);
	assert(close_calls == 1 && remove_calls == 1 && notifications == 0);
	assert(fatal_errors == fatal_on_error && !file_open && !file_exists);
}

static void test_close_failure(void)
{
	reset_test(test_score_path, TEST_SMALL_FILE_SIZE);
	fail_close = 1;
	legacy_s16 result = file_write_fatal(test_score_path, payload, expected_length);
	assert(close_calls == 1 && written_length == expected_length);
	assert(!file_open && notifications == 0 && fatal_errors == 0);
#ifdef __ANDROID__
	assert(result != 0 && remove_calls == 1 && !file_exists);
#else
	/* Keep the original DOS-compatible close behavior on other platforms. */
	assert(result == 0 && remove_calls == 0 && file_exists);
#endif
}

legacy_int main(void)
{
	for (legacy_u32 index = 0; index < sizeof(payload); index++) {
		payload[index] = (legacy_u8)(index * TEST_PATTERN_MULTIPLIER + (index >> LEGACY_BYTE_BITS));
	}
	test_successful_save(test_track_path, TEST_SMALL_FILE_SIZE);
	test_successful_save(test_score_path, TEST_SMALL_FILE_SIZE);
	test_successful_save(test_replay_path, sizeof(payload));
	test_successful_save(test_track_path, 0);
	test_failed_create();
	test_short_write(0);
	test_short_write(1);
	test_close_failure();
	puts("File write completion, notification and failure regressions passed");
	return 0;
}
