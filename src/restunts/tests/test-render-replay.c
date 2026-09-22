#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../c/externs.h"
#include "../c/restunts.h"
#include "../c/fileio.h"
#include "../c/memmgr.h"
#include "../c/platform.h"
#include "../c/frame_internal.h"
#include "../c/frame_prediction.h"
#include "../c/game_input.h"
#include "../c/keyboard.h"
#include "../c/race_resources.h"
#include "../c/residue.h"
#include "../c/track_collision.h"
#include "../c/wheel_transform.h"
#include "../c/camera.h"
#include "../c/shape2d.h"
#include "../c/track_objects.h"
#include "../c/fatal.h"

#undef memcpy
#undef printf

struct RENDER_SCRATCH {
	struct TRACK_COLLISION_SNAPSHOT collision;
	struct LEGACY_EXECUTION_RESIDUE residue;
	struct VECTOR forward;
	struct VECTOR world;
	legacy_s16 plane;
	legacy_s16 heading;
	legacy_s16 pitch;
	legacy_s16 roll;
	legacy_s16 yaw;
	legacy_s16 render_headings;
};

static void capture_scratch(struct RENDER_SCRATCH *scratch)
{
	memset(scratch, 0, sizeof(*scratch));
	track_collision_capture(&scratch->collision);
	scratch->residue = legacy_execution_residue;
	scratch->forward = wheel_forward_travel;
	scratch->world = wheel_world_travel;
	scratch->plane = planindex_copy;
	scratch->heading = wheel_heading_offset;
	scratch->pitch = car_initial_pitch;
	scratch->roll = car_initial_roll;
	scratch->yaw = car_initial_yaw;
	scratch->render_headings = legacy_render_player_headings_active;
}

static void render_and_check(const struct GAMESTATE *predicted)
{
	struct GAMESTATE saved = state;
	struct GAMEINFO saved_config = gameconfig;
	legacy_s8 seed_before[GAMESTATE_RANDOM_SEED_SIZE];
	legacy_s8 seed_after[GAMESTATE_RANDOM_SEED_SIZE];
	struct RENDER_SCRATCH before;
	struct RENDER_SCRATCH after;
	get_kevinrandom_seed(seed_before);
	capture_scratch(&before);
	sprite_select_render_window();
	if (predicted != NULL) {
		update_frame_predicted(0, &rect_windshield, predicted, NULL, NULL);
	} else if (supersight_enabled != 0) {
		update_frame_predicted(0, &rect_windshield, &saved, NULL, NULL);
	} else {
		update_frame(0, &rect_windshield);
	}
	frame_present(&rect_windshield);
	get_kevinrandom_seed(seed_after);
	assert(memcmp(&saved, &state, sizeof(state)) == 0);
	assert(memcmp(&saved_config, &gameconfig, sizeof(gameconfig)) == 0);
	assert(memcmp(seed_before, seed_after, sizeof(seed_before)) == 0);
	if (predicted != NULL || supersight_enabled != 0) {
		capture_scratch(&after);
		assert(memcmp(&before, &after, sizeof(before)) == 0);
	}
	if (full_redraw_frames_remaining != 0) {
		full_redraw_frames_remaining--;
	}
}

static void initialize_replay(const legacy_s8 *name)
{
	assert(file_load_replay("", name) == 0);
	replay_filename[0] = 0;
	gameconfigcopy = gameconfig;
	assert(track_setup() == 0);
	cvxptr = mmgr_alloc_resbytes("cvx", sizeof(struct GAMESTATE) * GAMESTATE_CHECKPOINT_COUNT);
	init_game_state(-1);
	viewport_bottom_cache = -1;
	run_game_random = LEGACY_S16_SHL(get_kevinrandom(), 3U);
	replaybar_toggle = 0;
	is_in_replay = 1;
	idle_expired = 0;
	cameramode = CAMERA_MODE_COCKPIT;
	game_replay_mode = REPLAY_MODE_PLAYBACK;
	detail_level = 0;
	slow_video_mgmt = slow_video_mgmt_copy = 1;
	assert(setup_player_cars_without_dashboard() == 0);
	kbormouse = 0;
	replay_playback_speed = REPLAY_PLAYBACK_NORMAL;
	race_exit_request = 1;
	game_replay_mode_copy = -1;
	frame_buffer_index = dashboard_buffer_index = 0;
	recording_limit_warning_requested = 0;
	dashb_toggle = 0;
	followOpponentFlag = 0;
	framespersec = gameconfig.game_framespersec;
	start_flag_animation = 500;
	rect_windshield = (struct RECTANGLE){0, 320, 0, 200};
	set_projection(35, 200 / 6, 320, 200);
	restore_gamestate(0);
	init_rect_arrays();
	full_redraw_frames_remaining = 1;
	/* This is an ordinary live renderer, with no archived dump stack emulation. */
	shape3d_set_legacy_render_stack(NULL, 0, 0, NULL);
}

legacy_s16 stuntsmain(legacy_s16 argc, legacy_s8 *argv[])
{
	assert(argc == 5);
	legacy_s16 mode = (legacy_s16)atoi((const char *)argv[3]);
	legacy_u16 limit = (legacy_u16)atoi((const char *)argv[4]);
	assert(mode >= 0 && mode <= 2);
	init_main(argc, argv);
	init_div0();
	init_row_tables();
	mainresptr = file_load_resfile("main");
	fontdefptr = file_load_resource(0, "fontdef.fnt");
	fontnptr = file_load_resource(0, "fontn.fnt");
	font_set_fontdef();
	init_polyinfo();
	init_trackdata();
	reset_race_loop_state();
	init_kevinrandom("kevin");
	initialize_replay(argv[1]);
	if (limit == 0 || limit > gameconfig.game_recordedframes) {
		limit = gameconfig.game_recordedframes;
	}
	legacy_u8 *recording = malloc(gameconfig.game_recordedframes);
	assert(recording != NULL);
	memcpy(recording, replay_input_buffer, gameconfig.game_recordedframes);
	FILE *output = fopen((const char *)argv[2], "wb");
	assert(output != NULL);
	struct GAMESTATE previous = state;
	legacy_u32 extra_frames = 0;
	for (legacy_u16 tick = 0; tick <= limit; tick++) {
		assert((legacy_u16)state.game_frame == tick);
		legacy_s16 enhanced = mode == 1 || (mode == 2 && (tick / 17U) % 2U != 0);
		if (enhanced != supersight_enabled) {
			assert(handle_ingame_kb_shortcuts(KEY_F12) != 0);
		}
		/* Cover each camera and both targets without modifying recorded input. */
		cameramode = (legacy_s8)((tick / 29U) % CAMERA_MODE_COUNT);
		followOpponentFlag = gameconfig.game_opponenttype != 0 && (tick / 13U) % 2U != 0;
		render_and_check(NULL);
		if (enhanced != 0 && tick != 0) {
			legacy_u16 count = 60U / gameconfig.game_framespersec;
			for (legacy_u16 frame = 1; frame < count; frame++) {
				struct GAMESTATE predicted;
				frame_predict_state(&predicted, &state, &previous, (65536UL * frame) / count);
				render_and_check(&predicted);
				extra_frames++;
			}
		}
		legacy_u8 bytes[GAMESTATE_SERIALIZED_SIZE];
		assert(gamestate_serialize(bytes, &state) == sizeof(bytes));
		assert(fwrite(bytes, 1, sizeof(bytes), output) == sizeof(bytes));
		assert(memcmp(recording, replay_input_buffer, gameconfig.game_recordedframes) == 0);
		if (tick < limit) {
			previous = state;
			update_gamestate();
		}
	}
	assert(mode == 0 ? extra_frames == 0 : extra_frames != 0);
	assert(fclose(output) == 0);
	free(recording);
	/* Match pixldump shutdown: this fixture never loaded dashboard resources. */
	call_exitlist();
	return 0;
}
