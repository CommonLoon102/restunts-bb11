// Requires the original DEFAULT.RPL and game resources in C:/DATA.
// Executes the translated physics and serializers against an independent
// archived DOS assembly oracle. Run from a fresh disposable guest task.
#include "LOAD.HC"
#include "REPLAY_EXPECTED.HH"

#define TEMPLE_REPLAY_DEBUG_PORT 0xE9
#define TEMPLE_REPLAY_FNV_OFFSET 2166136261
#define TEMPLE_REPLAY_FNV_PRIME 16777619
#define TEMPLE_REPLAY_DUMP_HEADER 2
#define TEMPLE_REPLAY_BODY_BYTES (TEMPLE_REPLAY_TEST_FRAMES * TEMPLE_REPLAY_STATE_BYTES)
#define TEMPLE_REPLAY_TRACE_BYTES (TEMPLE_REPLAY_DUMP_HEADER + TEMPLE_REPLAY_BODY_BYTES)
#define TEMPLE_REPLAY_TRACK_BYTES 1802
#define TEMPLE_REPLAY_CHECKPOINT_BYTES 81
#define TEMPLE_REPLAY_POLYINFO_BYTES 10400
#define TEMPLE_REPLAY_CVX_BYTES 22400
#define TEMPLE_REPLAY_CAR_ID_BYTES 4
#define TEMPLE_REPLAY_CAR_PREFIX_BYTES 3
#define TEMPLE_REPLAY_RANDOM_SHIFT 3
#define TEMPLE_REPLAY_START_TIMER 500
#define TEMPLE_REPLAY_CALLER_SI 80
#define TEMPLE_REPLAY_INPUT_PATH "C:/DATA/DEFAULT.RPL"
#define TEMPLE_REPLAY_TRACE_PATH "C:/REPLAY.BNI"
#define TEMPLE_REPLAY_FAILURE_PATH "C:/FAILURE.BIN"

// Test-only diagnostics remain available if a game error dialog waits for input.
legacy_void TempleReplayProgress(legacy_u8 *message)
{
	while (*message) {
		OutU8(TEMPLE_REPLAY_DEBUG_PORT, *message++);
	}
	OutU8(TEMPLE_REPLAY_DEBUG_PORT, '\n');
}

legacy_u32 TempleReplayHash(legacy_u8 *bytes, legacy_u64 count)
{
	legacy_u64 index, hash = TEMPLE_REPLAY_FNV_OFFSET;
	for (index = 0; index < count; index++) {
		hash = ((hash ^ bytes[index]) * TEMPLE_REPLAY_FNV_PRIME) & U32_MAX;
	}
	return hash;
}

legacy_void TempleReplayAssert(legacy_u8 condition, legacy_u8 *message)
{
	if (!condition) {
		PrintErr("Replay test: %s\n", message);
		throw('Replay');
	}
}

legacy_void TempleReplayCarResources()
{
	legacy_u64 index;
	legacy_u8 *resource;
	setup_legacy_penalty_route_word;
	for (index = 0; index < TEMPLE_REPLAY_CAR_ID_BYTES; index++) {
		car_resource_name[index + TEMPLE_REPLAY_CAR_PREFIX_BYTES] =
			gameconfig.game_playercarid[index];
	}
	resource = file_load_resfile(car_resource_name);
	setup_aero_trackdata(resource, PLAYER_CAR_INDEX);
	unload_resource(resource);
	if (gameconfig.game_opponenttype) {
		for (index = 0; index < TEMPLE_REPLAY_CAR_ID_BYTES; index++) {
			car_resource_name[index + TEMPLE_REPLAY_CAR_PREFIX_BYTES] =
				gameconfig.game_opponentcarid[index];
		}
		resource = file_load_resfile(car_resource_name);
		setup_aero_trackdata(resource, OPPONENT_CAR_INDEX);
		unload_resource(resource);
		load_opponent_data;
	}
	load_track_collision_resources;
	followOpponentFlag = FALSE;
	is_in_replay_copy = -1;
}

legacy_void TempleReplaySetup()
{
	legacy_s8 *arguments[1];
	legacy_u64 index;
	arguments[0] = "TEMPLE_REPLAY_TEST";
	TempleReplayProgress("REPLAY: data initialization");
	full_data_initialize;
	configure_powergear_bug(1, arguments);
	configure_legacy_collision(1, arguments);
	configure_left_corner_bias(1, arguments);
	configure_owoot(1, arguments);
	init_video_geometry_flags;
	video_uses_page_flipping = FALSE;
	video_page_count = 1;
	textresprefix = 'e';
	framespersec = GAME_FRAME_RATE_NORMAL;
	TempleReplayProgress("REPLAY: memory arena");
	mmgr_init_conventional_arena;
	init_div0;
	init_row_tables;
	polyinfoptr = mmgr_alloc_resbytes("polyinfo", TEMPLE_REPLAY_POLYINFO_BYTES);
	TempleReplayProgress("REPLAY: track storage");
	init_trackdata;
	reset_race_loop_state;
	init_kevinrandom("kevin");
	TempleReplayProgress("REPLAY: load DEFAULT.RPL");
	file_load_replay("", "DEFAULT");
	MemCpy(&gameconfigcopy, &gameconfig, sizeof(GAMEINFO));
	for (index = 0; index < TEMPLE_REPLAY_TRACK_BYTES; index++) {
		track_and_directory_backup[index] = track_element_map[index];
	}
	for (index = 0; index < TEMPLE_REPLAY_CHECKPOINT_BYTES; index++) {
		track_and_directory_backup[index + TEMPLE_REPLAY_TRACK_BYTES] = track_directory[index];
		track_and_directory_backup[index + TEMPLE_REPLAY_TRACK_BYTES +
								   TEMPLE_REPLAY_CHECKPOINT_BYTES] = replay_directory[index];
	}
	TempleReplayProgress("REPLAY: track setup");
	track_setup;
	cvxptr = mmgr_alloc_resbytes("cvx", TEMPLE_REPLAY_CVX_BYTES);
	TempleReplayProgress("REPLAY: initial state");
	init_game_state(-1);
	viewport_bottom_cache = -1;
	run_game_random = TempleCastS16(get_kevinrandom() << TEMPLE_REPLAY_RANDOM_SHIFT);
	replaybar_toggle = TRUE;
	is_in_replay = TRUE;
	idle_expired = FALSE;
	cameramode = 0;
	game_replay_mode = REPLAY_MODE_PLAYBACK;
	TempleReplayProgress("REPLAY: car resources");
	TempleReplayCarResources;
	kbormouse = FALSE;
	replay_playback_speed = REPLAY_PLAYBACK_NORMAL;
	race_exit_request = TRUE;
	game_replay_mode_copy = TEMPLE_BYTE_MASK;
	frame_buffer_index = 0;
	dashboard_buffer_index = 0;
	recording_limit_warning_requested = FALSE;
	dashb_toggle = FALSE;
	start_flag_animation = TEMPLE_REPLAY_START_TIMER;
	framespersec = GAME_FRAME_RATE_NORMAL;
	TempleReplayProgress("REPLAY: restore initial state");
	restore_gamestate(0);
	TempleReplayProgress("REPLAY: restore replay state");
	restore_gamestate(gameconfig.game_recordedframes);
}

legacy_void TempleReplayTest()
{
	CDirEntry trace_entry;
	legacy_s64 input_size = 0;
	legacy_u8 *input = FileRead(TEMPLE_REPLAY_INPUT_PATH, &input_size);
	legacy_u8 *trace;
	legacy_u8 *serialized;
	legacy_u64 frame;
	legacy_u32 hash;
	TempleReplayAssert(input && input_size == TEMPLE_REPLAY_INPUT_BYTES,
					   "original DEFAULT.RPL size");
	TempleReplayAssert(TempleReplayHash(input, input_size) == TEMPLE_REPLAY_INPUT_HASH,
					   "original DEFAULT.RPL fingerprint");
	Free(input);
	TempleReplayAssert(Cd("C:/DATA"), "data directory");
	TempleReplayProgress("REPLAY: setup");
	TempleReplaySetup;
	TempleReplayAssert(state.game_frame == 0, "initial frame");
	TempleReplayProgress("REPLAY: physics frames");
	trace = MAlloc(TEMPLE_REPLAY_TRACE_BYTES);
	TempleReplayProgress("REPLAY: trace allocated");
	trace[0] = TEMPLE_REPLAY_TEST_FRAMES & TEMPLE_BYTE_MASK;
	trace[1] = TEMPLE_REPLAY_TEST_FRAMES >> TEMPLE_BYTE_BITS;
	try {
		for (frame = 0; frame < TEMPLE_REPLAY_TEST_FRAMES; frame++) {
			// Same checkpoint behavior as update_gamestate, with audio disabled.
			rs_gamestep_update_gamestate_impl(TEMPLE_REPLAY_CALLER_SI, FALSE, TRUE);
			if (!(frame % GAME_FRAME_RATE_NORMAL)) {
				TempleReplayProgress("REPLAY: frame updated");
			}
			serialized = trace + TEMPLE_REPLAY_DUMP_HEADER + frame * TEMPLE_REPLAY_STATE_BYTES;
			TempleReplayAssert(gamestate_serialize(serialized, &state) == TEMPLE_REPLAY_STATE_BYTES,
							   "canonical serialized size");
			if (!(frame % GAME_FRAME_RATE_NORMAL)) {
				TempleReplayProgress("REPLAY: frame serialized");
			}
			hash = TempleReplayHash(serialized, TEMPLE_REPLAY_STATE_BYTES);
			if (hash != temple_replay_expected[frame]) {
				FileWrite(TEMPLE_REPLAY_FAILURE_PATH, serialized, TEMPLE_REPLAY_STATE_BYTES);
				PrintErr("Replay frame %d differs: got %08X, DOS %08X\n", frame + 1, hash,
						 temple_replay_expected[frame]);
				throw('Replay');
			}
		}
		TempleReplayProgress("REPLAY: all frame hashes matched");
		if (!FileWrite(TEMPLE_REPLAY_TRACE_PATH, trace, TEMPLE_REPLAY_TRACE_BYTES) ||
			!FileFind(TEMPLE_REPLAY_TRACE_PATH, &trace_entry, FUF_JUST_FILES) ||
			trace_entry.size != TEMPLE_REPLAY_TRACE_BYTES) {
			TempleReplayAssert(FALSE, "canonical dump written");
		}
	} catch {
		Free(trace);
		// An uncaught HolyC exception continues unwinding after this cleanup.
		Fs->catch_except = FALSE;
	}
	Free(trace);
	Print("Replay: all %d frames match the original DOS state hashes.\n",
		  TEMPLE_REPLAY_TEST_FRAMES);
}

TempleReplayTest;
