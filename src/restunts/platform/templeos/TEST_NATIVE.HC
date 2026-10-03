// Run only in the disposable VM test image: creates and removes C:/NATIVE.TS*.
#include "LEGACY.HH"
#include "LIBC.HC"
#include "NATIVE.HH"

#define TEMPLE_TEST_PATH "C:/NATIVE.TST"
#define TEMPLE_TEST_SECOND_PATH "C:/NATIVE.TS2"
#define TEMPLE_TEST_NAME "NATIVE.TST"
#define TEMPLE_TEST_SECOND_NAME "NATIVE.TS2"
#define TEMPLE_TEST_ABSOLUTE_PATTERN "c:\\native.ts*"
#define TEMPLE_TEST_RELATIVE_PATTERN "..\\native.ts?"
#define TEMPLE_TEST_MISSING_PATTERN "c:/native.missing"
#define TEMPLE_TEST_FIRST_MATCH 1
#define TEMPLE_TEST_SECOND_MATCH 2
#define TEMPLE_TEST_ALL_MATCHES (TEMPLE_TEST_FIRST_MATCH | TEMPLE_TEST_SECOND_MATCH)
#define TEMPLE_TEST_BUFFER_SIZE 8
#define TEMPLE_TEST_KEY_F11 0x8500
#define TEMPLE_TEST_TICK_WAIT_MS 35
#define TEMPLE_TEST_PUMP_ROUNDS 4
#define TEMPLE_TEST_WORKER_KEY 'K'

legacy_s16 kb_parse_key(legacy_s16 value)
{
	return value;
}

legacy_u32 temple_test_audio_ticks = 0;
legacy_u32 temple_test_callback_ticks = 0;
legacy_u32 temple_test_worker_request = 0, temple_test_worker_acknowledged = 0;

legacy_void TempleAudioTick()
{
	temple_test_audio_ticks++;
}

legacy_void TempleVideoPresent()
{
}

legacy_void TempleVideoShutdown()
{
}

#include "FILE.HC"
#include "INPUT.HC"
#include "TIMER.HC"
#include "RUNTIME.HC"

legacy_void TempleTestAssert(legacy_u8 condition, legacy_u8 *message)
{
	if (!condition) {
		PrintErr("Native test: %s\n", message);
		throw('Native');
	}
}

legacy_void TempleTestCallback()
{
	temple_test_callback_ticks++;
}

legacy_void TempleTestFindPattern(legacy_u8 *pattern)
{
	legacy_u8 *directory = StrNew(Fs->cur_dir);
	legacy_u8 drive = Fs->cur_dv->drv_let;
	legacy_u8 *name = dos_file_find_first(pattern);
	legacy_u8 found = 0, match;
	while (name) {
		match = 0;
		if (!StrCmp(name, TEMPLE_TEST_NAME)) {
			match = TEMPLE_TEST_FIRST_MATCH;
		} else if (!StrCmp(name, TEMPLE_TEST_SECOND_NAME)) {
			match = TEMPLE_TEST_SECOND_MATCH;
		}
		TempleTestAssert(match != 0, "wildcard returns a matching basename");
		TempleTestAssert(!(found & match), "find next never repeats a file");
		found |= match;
		name = dos_file_find_next();
	}
	TempleTestAssert(found == TEMPLE_TEST_ALL_MATCHES, "wildcard enumerates both test files");
	TempleTestAssert(dos_file_find_next() == NULL, "exhausted search remains exhausted");
	TempleTestAssert(drive == Fs->cur_dv->drv_let && !StrCmp(directory, Fs->cur_dir),
					 "file search preserves the current drive and directory");
	Free(directory);
}

legacy_void TempleTestFileSearch()
{
	legacy_u16 handle = dos_file_open(TEMPLE_TEST_SECOND_PATH, TEMPLE_FILE_CREATE);
	legacy_u8 *name;
	TempleTestAssert(handle != TEMPLE_FILE_INVALID_HANDLE, "create second search fixture");
	TempleTestAssert(dos_file_write(handle, "S", 1) == 1, "write second search fixture");
	TempleTestAssert(!dos_file_close(handle), "persist second search fixture");
	TempleTestFindPattern(TEMPLE_TEST_ABSOLUTE_PATTERN);
	TempleTestFindPattern(TEMPLE_TEST_RELATIVE_PATTERN);
	TempleTestAssert(dos_file_find_first(TEMPLE_TEST_ABSOLUTE_PATTERN) != NULL,
					 "start a search before resetting it");
	TempleTestAssert(dos_file_find_first(TEMPLE_TEST_MISSING_PATTERN) == NULL,
					 "no-match search replaces previous results");
	TempleTestAssert(dos_file_find_next() == NULL, "no stale results after no match");
	name = dos_file_find_first(TEMPLE_TEST_PATH);
	TempleTestAssert(name != NULL, "exact search can restart after no match");
	TempleTestAssert(!StrCmp(name, TEMPLE_TEST_NAME), "exact search returns a basename");
	TempleTestAssert(dos_file_find_next() == NULL, "exact search has one result");
	TempleTestFindPattern(TEMPLE_TEST_ABSOLUTE_PATTERN);
	TempleFileShutdown();
	TempleTestAssert(dos_file_find_next() == NULL, "shutdown releases search results");
	TempleTestAssert(!dos_file_remove(TEMPLE_TEST_SECOND_PATH), "delete second search fixture");
}

legacy_void TempleTestPumpWorker(CTask *recipient)
{
	while (TRUE) {
		if (temple_test_worker_request != temple_test_worker_acknowledged) {
			TaskMsg(recipient, NULL, MSG_KEY_DOWN, TEMPLE_TEST_WORKER_KEY, 0, 0);
			temple_test_worker_acknowledged = temple_test_worker_request;
		}
		Yield();
	}
}

legacy_void TempleTestPumpScheduling()
{
	legacy_u16 round;
	legacy_u32 before;
	legacy_u8 progressed = TRUE, advanced = TRUE, delivered = TRUE;
	CTask *worker = Spawn(&TempleTestPumpWorker, Fs, "Pump regression", Fs->gs->num);
	TempleTestAssert(worker != NULL, "spawn cooperative worker");
	kb_init_interrupt();
	for (round = 0; round < TEMPLE_TEST_PUMP_ROUNDS; round++) {
		temple_test_worker_request++;
		/* Force an elapsed tick so timer_get_counter cannot yield through its
		 * idle Sleep branch. Busy drawing causes the same condition in game.
		 */
		temple_timer_last_tick = ToI64(tS * TEMPLE_TIMER_TICKS_PER_SECOND) - 1;
		before = temple_timer_counter;
		timer_get_counter();
		if (temple_test_worker_acknowledged != temple_test_worker_request) {
			progressed = FALSE;
		}
		if (temple_timer_counter <= before) {
			advanced = FALSE;
		}
		if (temple_key_count != 1 || temple_key_queue[temple_key_read] != TEMPLE_TEST_WORKER_KEY) {
			delivered = FALSE;
		}
		kb_init_interrupt();
	}
	Kill(worker);
	TempleTestAssert(progressed, "busy pump yields to another task");
	TempleTestAssert(advanced, "busy pump advances the timer");
	TempleTestAssert(delivered, "busy pump receives worker input messages");
}

legacy_void TempleTestNative()
{
	legacy_u16 handle;
	legacy_u8 buffer[TEMPLE_TEST_BUFFER_SIZE];
	legacy_u8 *path;
	legacy_u32 before;
	legacy_s16 y;
	legacy_s64 first, last;
	TempleTestAssert(sizeof(legacy_u16) == 2 && sizeof(legacy_u32) == 4 && sizeof(legacy_u8 *) == 8,
					 "fixed fields/native pointer sizes");
	path = TempleFilePath("c:\\mixed.res");
	TempleTestAssert(!StrCmp(path, "C:/MIXED.RES"), "DOS path normalization");
	Free(path);
	handle = dos_file_open(TEMPLE_TEST_PATH, TEMPLE_FILE_CREATE);
	TempleTestAssert(handle != TEMPLE_FILE_INVALID_HANDLE, "create");
	TempleTestAssert(dos_file_write(handle, "ABCD", 4) == 4, "write");
	TempleTestAssert(!dos_file_seek(handle, 1, TEMPLE_FILE_SEEK_BEGIN), "seek begin");
	TempleTestAssert(dos_file_write(handle, "Z", 1) == 1, "overwrite");
	TempleTestAssert(!dos_file_seek(handle, 2, TEMPLE_FILE_SEEK_END), "seek past end");
	TempleTestAssert(dos_file_write(handle, "Q", 1) == 1, "sparse write");
	TempleTestAssert(!dos_file_close(handle), "persist");
	handle = dos_file_open(TEMPLE_TEST_PATH, TEMPLE_FILE_OPEN_EXISTING);
	TempleTestAssert(handle != TEMPLE_FILE_INVALID_HANDLE, "reopen");
	MemSet(buffer, TEMPLE_BYTE_MASK, sizeof(buffer));
	TempleTestAssert(dos_file_read(handle, buffer, sizeof(buffer)) == 7, "short read");
	TempleTestAssert(!MemCmp(buffer, "AZCD\0\0Q", 7), "persisted bytes and zero-filled gap");
	TempleTestAssert(dos_file_read(handle, buffer, 1) == 0, "EOF");
	TempleTestAssert(dos_file_seek(handle, -1, TEMPLE_FILE_SEEK_BEGIN) == -1,
					 "negative seek rejected");
	TempleTestAssert(dos_file_error() == TEMPLE_FILE_INVALID_SEEK, "seek error code");
	TempleTestAssert(!dos_file_error(), "error read clears status");
	dos_file_close(handle);
	TempleTestFileSearch();
	TempleTestAssert(!dos_file_remove(TEMPLE_TEST_PATH), "delete test file");
	TempleTestAssert(!FileFind(TEMPLE_TEST_PATH), "file removed");
	TempleTestAssert(dos_file_open(TEMPLE_TEST_PATH, TEMPLE_FILE_OPEN_EXISTING) == 0,
					 "DOS missing-file handle contract");
	TempleTestAssert(dos_file_error() != 0 && !dos_file_error(), "missing-file status");
	TempleTestAssert(dos_file_read(TEMPLE_FILE_INVALID_HANDLE, buffer, sizeof(buffer)) == 0 &&
						 dos_file_error() != 0,
					 "DOS invalid-handle read contract");
	TempleTestAssert(TempleMouseX(TEMPLE_SCREEN_WIDTH - 1) == TEMPLE_LOGICAL_WIDTH - 1 &&
						 TempleMouseY(TEMPLE_SCREEN_HEIGHT - 1) == TEMPLE_LOGICAL_HEIGHT - 1,
					 "aspect-correct mouse mapping reaches all edges");
	TempleTestAssert(TempleInputKey(0, SC_F11) == TEMPLE_TEST_KEY_F11, "BIOS function-key mapping");
	for (y = 0; y < TEMPLE_LOGICAL_HEIGHT; y++) {
		first = y * TEMPLE_SCREEN_HEIGHT / TEMPLE_LOGICAL_HEIGHT;
		last = (y + 1) * TEMPLE_SCREEN_HEIGHT / TEMPLE_LOGICAL_HEIGHT - 1;
		TempleTestAssert(TempleMouseY(first) == y && TempleMouseY(last) == y,
						 "mouse hits match raster cell boundaries");
		dos_mouse_set_position(1, y);
		TempleTestAssert(TempleMouseX(ms.pos.x) == 1 && TempleMouseY(ms.pos.y) == y,
						 "mouse position round trip");
	}
	dos_timer_setup_interrupt();
	TempleTestAssert(dos_timer_register_callback(&TempleTestCallback), "register callback");
	Sleep(TEMPLE_TEST_TICK_WAIT_MS);
	TempleTimerPump();
	TempleTestAssert(temple_test_callback_ticks > 0 &&
						 temple_test_audio_ticks == temple_test_callback_ticks,
					 "timer/audio cadence");
	TempleTestPumpScheduling();
	dos_timer_set_callbacks_suspended(TRUE);
	before = temple_test_callback_ticks;
	Sleep(TEMPLE_TEST_TICK_WAIT_MS);
	TempleTimerPump();
	TempleTestAssert(temple_test_callback_ticks == before, "suspended callbacks");
	dos_timer_unregister_callback(&TempleTestCallback);
	dos_timer_shutdown();
	Print("Native file/input/timer/scheduling tests passed.\n");
}

TempleTestNative();
