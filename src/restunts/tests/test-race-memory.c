/* Include the complete entry module so this test uses the game's real startup
 * and retains its code/data footprint, including the menu and intro paths. */
#define run_main_menu_loop race_memory_original_main_menu_loop
#include "../c/restunts.c"
#undef run_main_menu_loop

#include "frame_internal.h"

static legacy_u32 race_memory_free_bytes;

static void race_memory_draw(void)
{
	init_game_state_with_frame_rate(configured_frame_rate);
	cameramode = CAMERA_MODE_COCKPIT;
	game_replay_mode = REPLAY_MODE_PAUSED;
	dashboard_visible = 1;
	dashb_toggle = 1;
	dashboard_buffer_index = 0;
	height_above_replaybar = GAME_SCREEN_HEIGHT;
	dashbmp_y_copy = dashbmp_y;
	roofbmpheight_copy = roofbmpheight;
	rect_windshield.left = 0;
	rect_windshield.right = GAME_SCREEN_WIDTH;
	rect_windshield.top = roofbmpheight;
	rect_windshield.bottom = dashbmp_y;
	set_projection(35, dashbmp_y / 6, GAME_SCREEN_WIDTH, dashbmp_y);
	for (legacy_s16 pass = 0; pass < 3; pass++) {
		supersight_enabled = pass == 1;
		frame_supersight_reset();
		init_rect_arrays();
		full_redraw_frames_remaining = 1;
		sprite_select_render_window_and_clear();
		setup_car_shapes(DASHBOARD_OPERATION_REDRAW_STATIC);
		update_frame(0, &rect_windshield);
		/* The same work buffer may serve the gearbox and instruments. Exercise
		 * successive knob draws, background restoration, and changing needles. */
		for (legacy_s16 step = 0; step < 3; step++) {
			state.playerstate.car_knob_x = simd_player.knob_points[pass + 1].px;
			state.playerstate.car_knob_y = simd_player.knob_points[pass + 1].py;
			state.playerstate.car_changing_gear = step != 1;
			state.playerstate.car_gear_change_delay = step != 1;
			state.playerstate.car_rev_speed = (legacy_u16)((pass * 3 + step) * 10) << 8;
			state.playerstate.car_currpm = (pass * 3 + step) * 600;
			full_redraw_frames_remaining = step == 0;
			setup_car_shapes(DASHBOARD_OPERATION_UPDATE);
		}
	}
}

static legacy_s16 race_memory_check(void)
{
	init_main_input_state();
	set_default_car();
	_memcpy(gameconfig.game_playercarid, "DIA3", 4);
	_memcpy(gameconfig.game_opponentcarid, "CSIL", 4);
	gameconfig.game_opponenttype = 1;
	detail_level = 0;
	configured_frame_rate = GAME_FRAME_RATE_NORMAL;
	framespersec = GAME_FRAME_RATE_NORMAL;
	strcpy(gameconfig.game_trackname, "DEFAULT");
	idle_expired = 0;
	file_build_path(track_directory, gameconfig.game_trackname, ".trk", g_path_buf);
	file_read_fatal(g_path_buf, track_element_map);
	main_menu_backup_track();
	if (main_menu_prepare_track() != 1) {
		return 1;
	}

	cvxptr = mmgr_alloc_resbytes("cvx", sizeof(struct GAMESTATE) * GAMESTATE_CHECKPOINT_COUNT);
	init_game_state(GAMESTATE_INIT_RESET_CHECKPOINTS);
	gameconfig.game_recordedframes = 0;
	show_waiting();
	legacy_s16 result = setup_player_cars();
	race_memory_free_bytes = mmgr_get_res_ofs_diff_scaled();
	if (result == 0) {
		/* Demand the actual full-size buffer and the loaded cockpit. The replay
		 * dump path deliberately omits the dashboard and cannot cover this bug. */
		if (video_uses_page_flipping != 0 || render_window_sprite == 0 || stdaresptr == 0 ||
			stdbresptr == 0 || dashboard_instrument_sprite == 0 ||
			shape2d_get_width(render_window_sprite->sprite_bitmapptr) != GAME_SCREEN_WIDTH ||
			shape2d_get_height(render_window_sprite->sprite_bitmapptr) != GAME_SCREEN_HEIGHT) {
			result = 1;
		} else {
			race_memory_draw();
		}
	}
	free_player_cars();
	mmgr_release(cvxptr);
	return result;
}

legacy_s16 run_main_menu_loop(legacy_s16 argc, legacy_s8 *argv[])
{
	init_full_game(argc, argv);
	legacy_s16 result = race_memory_check();
	if (result == 0) {
		/* Repeat the complete resource lifetime, including shared window release. */
		legacy_u32 first_free_bytes = race_memory_free_bytes;
		result = race_memory_check();
		if (race_memory_free_bytes < first_free_bytes) {
			result = 1;
		}
	}
	shutdown_dos_game();
	legacy_s8 remaining[] = "Free bytes after race allocation: ";
	dos_write_stdout(remaining, sizeof(remaining) - 1);
	legacy_s8 digits[11];
	legacy_u16 length = 0;
	do {
		digits[length++] = (legacy_s8)('0' + race_memory_free_bytes % 10UL);
		race_memory_free_bytes /= 10UL;
	} while (race_memory_free_bytes != 0);
	while (length != 0) {
		dos_write_stdout(&digits[--length], 1);
	}
	dos_write_stdout("\r\n", 2);
	if (result == 0) {
		legacy_s8 success[] = "DIA3 / CSIL race memory check passed\r\n";
		dos_write_stdout(success, sizeof(success) - 1);
	} else {
		legacy_s8 failure[] = "DIA3 / CSIL race memory check failed\r\n";
		dos_write_stdout(failure, sizeof(failure) - 1);
	}
	return result;
}
