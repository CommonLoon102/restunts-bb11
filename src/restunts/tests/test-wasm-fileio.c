#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <emscripten.h>
#include "../c/platform.h"
#include "../c/fileio.h"
#include "../c/fatal.h"

enum {
	TEST_PERSIST_DELAY_MS = 1,
	TEST_NO_COMMITS = 0,
	TEST_FIRST_COMMIT = 1,
	TEST_SECOND_COMMIT = 2,
	TEST_THIRD_COMMIT = 3,
	TEST_DELETE_COMMIT = 4,
	TEST_GAME_COMMIT = 5
};

static const legacy_s8 test_path[] = "Save.TrK";
static const legacy_s8 test_path_alternate_case[] = "sAvE.tRk";
static const legacy_s8 test_missing_path[] = "missing.trk";
static legacy_char test_payload[] = "track payload";
static legacy_u16 fatal_errors;
static const legacy_s8 test_scratch_path[] = "GH012345.TMP";
static legacy_u16 scratch_handle;

/* The callback deliberately resolves on a later event-loop turn. Capturing
 * MEMFS bytes here verifies that fclose flushed them before persistence, and
 * that a deletion leaves the virtual file present until the commit succeeds. */
/* clang-format parses these JavaScript operators as C tokens. */
// clang-format off
EM_JS(void, install_persistence, (legacy_int delay_ms), {
	Module['commits'] = [];
	Module['persistenceAttempts'] = 0;
	Module['rejectPersistence'] = false;
	Module['persistFile'] = async (path, removed) => {
		Module['persistenceAttempts']++;
		await new Promise(resolve => setTimeout(resolve, delay_ms));
		if (Module['rejectPersistence']) {
			throw new Error('Expected persistence failure');
		}
		const data = FS.readFile(path, { encoding: 'utf8' });
		Module['commits'].push({ path, removed, data });
	};
});

EM_JS(void, reject_persistence, (legacy_int rejected), {
	Module['rejectPersistence'] = rejected != 0;
});

EM_JS(legacy_int, commit_count, (void), {
	return Module['commits'].length;
});

EM_JS(legacy_int, persistence_attempt_count, (void), {
	return Module['persistenceAttempts'];
});

EM_JS(legacy_int, last_commit_matches,
	(const legacy_char *path, const legacy_char *data, legacy_int removed), {
	const commit = Module['commits'][Module['commits'].length - 1];
	return commit !== undefined && commit.path === UTF8ToString(path) &&
		commit.data === UTF8ToString(data) && commit.removed === (removed != 0);
});
EM_JS(void, install_synchronous_cleanup, (void), {
	Module['persistFileNeeded'] = path => !path.endsWith('.TMP');
});
// clang-format on

void fatal_error(const legacy_s8 *format, ...)
{
	(void)format;
	fatal_errors++;
}

static legacy_u16 create_file(void)
{
	legacy_u16 handle = dos_file_open(test_path, DOS_FILE_CREATE);
	assert(handle != 0);
	return handle;
}

static void write_payload(legacy_u16 handle)
{
	const legacy_u16 length = sizeof(test_payload) - 1;
	assert(dos_file_write(handle, test_payload, length) == length);
}

static void test_close_commits(void)
{
	legacy_u16 handle = create_file();
	assert(commit_count() == TEST_NO_COMMITS);
	assert(dos_file_close(handle) == 0);
	assert(commit_count() == TEST_FIRST_COMMIT);
	assert(last_commit_matches((const legacy_char *)test_path, "", 0));

	handle = create_file();
	write_payload(handle);
	assert(dos_file_seek(handle, 0, DOS_FILE_SEEK_BEGIN) == 0);
	legacy_char readback[sizeof(test_payload)] = {0};
	assert(dos_file_read(handle, readback, sizeof(test_payload) - 1) == sizeof(test_payload) - 1);
	assert(strcmp(readback, test_payload) == 0);
	assert(dos_file_close(handle) == 0);
	assert(commit_count() == TEST_SECOND_COMMIT);
	assert(last_commit_matches((const legacy_char *)test_path, test_payload, 0));

	/* Closing a reused read-only handle must not replay its prior write. */
	handle = dos_file_open(test_path_alternate_case, DOS_FILE_OPEN_EXISTING);
	assert(handle != 0);
	assert(dos_file_close(handle) == 0);
	assert(commit_count() == TEST_SECOND_COMMIT);

	/* A zero-byte save still truncates the existing persistent file. */
	handle = create_file();
	assert(dos_file_close(handle) == 0);
	assert(commit_count() == TEST_THIRD_COMMIT);
	assert(last_commit_matches((const legacy_char *)test_path, "", 0));
}

static void test_failed_close_and_delete(void)
{
	reject_persistence(1);
	legacy_u16 handle = create_file();
	write_payload(handle);
	assert(dos_file_close(handle) == -1);
	assert(dos_file_error() != 0);
	assert(dos_file_error() == 0);
	assert(commit_count() == TEST_THIRD_COMMIT);

	/* Disk failure must leave MEMFS data available for recovery/export. */
	assert(dos_file_remove(test_path_alternate_case) == -1);
	assert(dos_file_error() != 0);
	struct stat info;
	assert(stat((const legacy_char *)test_path, &info) == 0);
	assert((legacy_s64)info.st_size == (legacy_s64)sizeof(test_payload) - 1);
	assert(commit_count() == TEST_THIRD_COMMIT);

	reject_persistence(0);
	assert(dos_file_remove(test_path_alternate_case) == 0);
	assert(commit_count() == TEST_DELETE_COMMIT);
	assert(last_commit_matches((const legacy_char *)test_path, test_payload, 1));
	assert(stat((const legacy_char *)test_path, &info) != 0);

	assert(dos_file_remove(test_missing_path) == -1);
	assert(dos_file_error() != 0);
	assert(commit_count() == TEST_DELETE_COMMIT);
}

static void test_game_write_result(void)
{
	const legacy_u32 length = sizeof(test_payload) - 1;
	assert(file_write_fatal(test_path, test_payload, length) == 0);
	assert(commit_count() == TEST_GAME_COMMIT);
	assert(last_commit_matches((const legacy_char *)test_path, test_payload, 0));

	reject_persistence(1);
	legacy_int attempts = persistence_attempt_count();
	assert(file_write_fatal(test_path, test_payload, length) != 0);
	assert(fatal_errors == 0);
	assert(persistence_attempt_count() == attempts + 1);
	assert(commit_count() == TEST_GAME_COMMIT);
	struct stat info;
	assert(stat((const legacy_char *)test_path, &info) == 0);
	assert((legacy_s64)info.st_size == length);

	/* Retain the historical inverted fatal flag while reporting the commit
	 * error. Neither error path may delete a prior persistent save. */
	attempts = persistence_attempt_count();
	assert(file_write_nofatal(test_path, test_payload, length) != 0);
	assert(fatal_errors == 1);
	assert(persistence_attempt_count() == attempts + 1);
	assert(stat((const legacy_char *)test_path, &info) == 0);
}

static void close_scratch_at_exit(void)
{
	legacy_int attempts = persistence_attempt_count();
	assert(dos_file_close(scratch_handle) == 0);
	assert(dos_file_remove(test_scratch_path) == 0);
	assert(persistence_attempt_count() == attempts);
	puts("WebAssembly synchronous exit cleanup regressions passed");
}

static void test_synchronous_exit_cleanup(void)
{
	install_synchronous_cleanup();
	scratch_handle = dos_file_open(test_scratch_path, DOS_FILE_CREATE);
	assert(scratch_handle != 0);
	write_payload(scratch_handle);
	/* Asyncify is shut down before exit handlers run. A scratch close or
	 * removal must not enter even an immediately resolved async callback. */
	assert(atexit(close_scratch_at_exit) == 0);
}

legacy_int main(void)
{
	install_persistence(TEST_PERSIST_DELAY_MS);
	test_close_commits();
	test_failed_close_and_delete();
	test_game_write_result();
	test_synchronous_exit_cleanup();
	puts("WebAssembly asynchronous file persistence regressions passed");
	return 0;
}
