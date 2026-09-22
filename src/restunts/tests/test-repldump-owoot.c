#include <assert.h>
#include <string.h>

#define RESTUNTS_HEADLESS
#include "../repldump/repldump.c"

#undef memcpy
#undef strcmp

struct GAMESTATE state;
legacy_s16 owoot_enabled;

static legacy_u32 open_count, write_count, close_count, remove_count;
static legacy_u32 output_length;
static legacy_u8 output_bytes[8];
static legacy_s32 output_exists, create_fails, write_is_short, close_fails;
static const char *expected_name;

legacy_u16 dos_file_open(const legacy_s8 *path, legacy_s16 create)
{
	assert(strcmp((const char *)path, expected_name) == 0);
	assert(create == DOS_FILE_CREATE);
	open_count++;
	if (create_fails) {
		return 0;
	}
	output_exists = 1;
	output_length = 0;
	return 5;
}

legacy_u16 dos_file_write(legacy_u16 handle, const void far *source, legacy_u16 length)
{
	assert(handle == 5);
	assert(output_exists);
	assert(length == 4);
	write_count++;
	legacy_u16 written = write_is_short ? length - 1 : length;
	memcpy(output_bytes, source, written);
	output_length = written;
	return written;
}

legacy_s16 dos_file_close(legacy_u16 handle)
{
	assert(handle == 5);
	close_count++;
	return close_fails ? -1 : 0;
}

legacy_s16 dos_file_remove(const legacy_s8 *path)
{
	assert(strcmp((const char *)path, expected_name) == 0);
	remove_count++;
	output_exists = 0;
	return 0;
}

static void reset_output(const char *name)
{
	memset(&state, 0, sizeof(state));
	memset(output_bytes, 0xcc, sizeof(output_bytes));
	open_count = write_count = close_count = remove_count = output_length = 0;
	output_exists = create_fails = write_is_short = close_fails = 0;
	expected_name = name;
	owoot_enabled = 1;
}

static void assert_result(const char *result)
{
	assert(output_exists && output_length == 4);
	assert(memcmp(output_bytes, result, 4) == 0);
	assert(output_bytes[4] == 0xcc);
	assert(open_count == 1 && write_count == 1 && close_count == 1 && remove_count == 1);
}

static void test_replay_extensions(void)
{
	static const char *names[] = {"R0000.rpl", "R0000.RPL", "R0000.RpL", "R0000"};
	for (legacy_u32 index = 0; index < sizeof(names) / sizeof(names[0]); index++) {
		legacy_s8 name[REPLDUMP_OUTPUT_NAME_SIZE];
		strcpy(name, names[index]);
		repldump_strip_replay_extension(name);
		assert(strcmp((const char *)name, "R0000") == 0);
	}
	legacy_s8 name[] = "R0000.rpx";
	repldump_strip_replay_extension(name);
	assert(strcmp((const char *)name, "R0000.rpx") == 0);
}

static void test_output_names(void)
{
	struct {
		legacy_u8 before;
		legacy_s8 name[REPLDUMP_OUTPUT_NAME_SIZE];
		legacy_u8 after;
	} output;
	memset(&output, 0xcc, sizeof(output));
	assert(repldump_output_name(output.name, "12345678", ".owo"));
	assert(strcmp((const char *)output.name, "12345678.owo") == 0);
	assert(output.before == 0xcc && output.after == 0xcc);
	assert(repldump_output_name(output.name, "R0000", ".BNI"));
	assert(strcmp((const char *)output.name, "R0000.BNI") == 0);
	memset(output.name, 0xcc, sizeof(output.name));
	assert(!repldump_output_name(output.name, "123456789", ".owo"));
	for (legacy_u32 index = 0; index < sizeof(output.name); index++) {
		assert((legacy_u8)output.name[index] == 0xcc);
	}
	assert(output.before == 0xcc && output.after == 0xcc);
	reset_output("123456789.owo");
	assert(!repldump_write_owoot_result("123456789", 1));
	assert(open_count == 0 && write_count == 0 && close_count == 0 && remove_count == 0);
}

static void test_result_contents(void)
{
	reset_output("R0000.owo");
	assert(repldump_write_owoot_result("R0000", 1));
	assert_result("pass");
	reset_output("R0019.owo");
	/* A new validation must replace a successful verdict from an earlier run. */
	output_exists = 1;
	output_length = 4;
	memcpy(output_bytes, "pass", 4);
	assert(repldump_write_owoot_result("R0019", 0));
	assert_result("fail");
}

static void test_completion_requires_finish(void)
{
	static const legacy_s8 events[] = {CRASH_EVENT_NONE,  CRASH_EVENT_COLLISION,
									   CRASH_EVENT_WATER, CRASH_EVENT_FINISH,
									   CRASH_EVENT_EXIT,  CRASH_EVENT_IMMEDIATE_STOP};
	for (legacy_u32 index = 0; index < sizeof(events) / sizeof(events[0]); index++) {
		reset_output("R0000.owo");
		state.playerstate.car_crashBmpFlag = events[index];
		assert(repldump_complete_owoot_result("R0000"));
		assert_result(events[index] == CRASH_EVENT_FINISH ? "pass" : "fail");
	}
}

static void test_disabled_does_not_touch_results(void)
{
	reset_output("R0000.owo");
	owoot_enabled = 0;
	assert(repldump_write_owoot_result("R0000", 0));
	assert(!output_exists);
	assert(open_count == 0 && write_count == 0 && close_count == 0 && remove_count == 0);
	output_exists = 1;
	output_length = 4;
	memcpy(output_bytes, "fail", 4);
	state.playerstate.car_crashBmpFlag = CRASH_EVENT_FINISH;
	assert(repldump_write_owoot_result("R0000", 1));
	assert(repldump_complete_owoot_result("R0000"));
	assert(output_exists && output_length == 4 && memcmp(output_bytes, "fail", 4) == 0);
	assert(open_count == 0 && write_count == 0 && close_count == 0 && remove_count == 0);
}

static void test_io_failures_remove_verdict(void)
{
	reset_output("R0000.owo");
	output_exists = 1;
	memcpy(output_bytes, "pass", 4);
	create_fails = 1;
	assert(!repldump_write_owoot_result("R0000", 1));
	assert(!output_exists);
	assert(open_count == 1 && write_count == 0 && close_count == 0 && remove_count == 1);

	reset_output("R0000.owo");
	write_is_short = 1;
	assert(!repldump_write_owoot_result("R0000", 1));
	assert(!output_exists);
	assert(open_count == 1 && write_count == 1 && close_count == 1 && remove_count == 2);

	reset_output("R0000.owo");
	close_fails = 1;
	assert(!repldump_write_owoot_result("R0000", 1));
	assert(!output_exists);
	assert(open_count == 1 && write_count == 1 && close_count == 1 && remove_count == 2);
}

int main(void)
{
	test_replay_extensions();
	test_output_names();
	test_result_contents();
	test_completion_requires_finish();
	test_disabled_does_not_touch_results();
	test_io_failures_remove_verdict();
	return 0;
}
