/* Full frame-loop endpoint traces captured before extraction cover waiting,
 * live/replay/paused input, exit requests and sequential drawing decisions. */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../c/race.c"
#undef printf
#undef memset

static legacy_u32 trace_hash = UINT32_C(2166136261);
static legacy_u32 scenario, frames, keys;
static legacy_u32 scripted_rewind;
static legacy_u32 presented_frames;

#ifdef RESTUNTS_SDL3
legacy_u8 supersight_enabled;
legacy_s16 camera_track_height_offset;
static legacy_u8 scheduled_mode;
static legacy_u8 scheduled_toggle;
static legacy_u64 scheduled_time;
static legacy_u32 scheduled_physics;
static legacy_u32 scheduled_ghosts;
static legacy_u32 scheduled_predictions;
static legacy_u32 scheduled_farthest_prediction;
static legacy_u8 scheduled_ghost_active;
static struct CARSTATE scheduled_ghost;
static struct GHOST_CAMERA_STATE scheduled_ghost_camera;

legacy_u64 presentation_now(void)
{
	return scheduled_time;
}

void SDL_Delay(Uint32 milliseconds)
{
	assert(scheduled_mode != 0);
	scheduled_time += (legacy_u64)milliseconds * 1000000U;
	assert(scheduled_time <= PRESENTATION_SECOND_NS);
}

void sdl3_platform_pump(void)
{
	assert(scheduled_mode != 0);
	legacy_u64 callbacks = scheduled_time * framespersec / PRESENTATION_SECOND_NS;
	if (game_replay_mode == REPLAY_MODE_PLAYBACK) {
		if (replay_playback_speed == REPLAY_PLAYBACK_SLOW) {
			callbacks /= 2;
		} else if (replay_playback_speed == REPLAY_PLAYBACK_FAST) {
			callbacks *= 2;
		}
	}
	elapsed_time2 = (legacy_u16)callbacks;
	if (scheduled_time == PRESENTATION_SECOND_NS) {
		race_exit_request = REPLAY_EXIT_REQUESTED;
	}
}

void sdl3_video_begin_frame(void)
{
}

void sdl3_video_end_frame(void)
{
}

struct CARSTATE *ghost_car_state(void)
{
	return scheduled_ghost_active != 0 ? &scheduled_ghost : NULL;
}

const struct GHOST_CAMERA_STATE *ghost_camera_state(void)
{
	return scheduled_ghost_active != 0 ? &scheduled_ghost_camera : NULL;
}

void update_frame_predicted(legacy_s8 buffer, struct RECTANGLE *rect,
							const struct GAMESTATE *predicted, const struct CARSTATE *ghost,
							const struct GHOST_CAMERA_STATE *ghost_camera)
{
	(void)buffer;
	(void)rect;
	assert(scheduled_mode != 0 && supersight_enabled != 0);
	assert(ghost == NULL && ghost_camera == NULL);
	assert(predicted->game_frame == state.game_frame);
	assert(predicted->playerstate.car_position.lx >= state.playerstate.car_position.lx);
	legacy_u32 distance =
		predicted->playerstate.car_position.lx - state.playerstate.car_position.lx;
	assert(distance <= (replay_playback_speed == REPLAY_PLAYBACK_FAST ? 120U : 60U));
	if (distance > scheduled_farthest_prediction) {
		scheduled_farthest_prediction = distance;
	}
	scheduled_predictions++;
	frames++;
}
#endif

void frame_supersight_reset(void)
{
}

void frame_fps_reset(void)
{
	presented_frames = 0;
}

void frame_fps_present_roof(void)
{
}

void frame_fps_record_presented(void)
{
	presented_frames++;
	assert(presented_frames == frames);
}
void ghost_update(legacy_u32 frame, legacy_u16 frame_rate)
{
#ifdef RESTUNTS_SDL3
	if (scheduled_mode != 0) {
		scheduled_ghosts++;
	}
#endif
	assert(frame == (game_replay_mode == REPLAY_MODE_PAUSED
						 ? 0
						 : (legacy_u32)(legacy_u16)state.game_frame + elapsed_time1));
	assert(frame_rate == (legacy_u16)framespersec);
}
static struct RECTANGLE dirty_rect;
static legacy_s8 text_resource[8];
static void trace(legacy_u32 value)
{
	for (legacy_u32 i = 0; i < 4; i++) {
		trace_hash = (trace_hash ^ (value & 255U)) * UINT32_C(16777619);
		value >>= 8;
	}
}
static void trace_rect(const struct RECTANGLE *rect)
{
	trace(rect->left);
	trace(rect->right);
	trace(rect->top);
	trace(rect->bottom);
}
/* Q-up polling and rewind-only dependencies must not alter the legacy trace. */
legacy_s16 kb_get_key_state(legacy_s16 scan_code)
{
	assert(scan_code == RACE_REWIND_SCAN_CODE);
	return scripted_rewind != 0 && frames < 2;
}
void dos_interrupts_disable(void)
{
	assert(scripted_rewind != 0);
}
void dos_interrupts_enable(void)
{
	assert(scripted_rewind != 0);
}
legacy_u32 timer_get_delta_alt(void)
{
	assert(scripted_rewind != 0);
	return 20;
}
void restore_gamestate(legacy_u16 target)
{
	assert(scripted_rewind != 0);
	state.game_frame = target;
	elapsed_time2 = target;
}
legacy_u8 dos_joystick_is_enabled(void)
{
	trace(1);
	return scenario & 1;
}
void replay_apply_analog_steering_history(void)
{
	trace(2);
}
void update_gamestate(void)
{
#ifdef RESTUNTS_SDL3
	if (scheduled_mode != 0) {
		scheduled_physics++;
		state.playerstate.car_position.lx += 60;
	}
#endif
	trace(3);
	state.game_frame++;
}
void init_rect_arrays(void)
{
	trace(4);
}
void input_push_status(void)
{
	trace(5);
}
void input_pop_status(void)
{
	trace(6);
}
void audio_suspend(void)
{
	trace(7);
}
void audio_resume(void)
{
	trace(8);
}
legacy_s8 far *locate_text_res(legacy_s8 far *resource, const legacy_s8 *name)
{
	(void)resource;
	trace(9);
	trace(name[0]);
	return text_resource;
}
legacy_u16 show_dialog(legacy_s16 type, legacy_s16 save, void far *text, legacy_u16 x, legacy_u16 y,
					   legacy_s16 color, legacy_s16 *flags, legacy_s16 selected)
{
	assert(text == text_resource);
	assert(flags == 0);
	trace(10);
	trace(type);
	trace(save);
	trace(x);
	trace(y);
	trace(color);
	trace(selected);
	return scenario & 1 ? 65535U : scenario & 2;
}
void dos_timer_set_callbacks_suspended(legacy_s16 value)
{
	trace(11);
	trace(value);
}
void update_crash_state(legacy_s16 event, legacy_s16 car)
{
	trace(12);
	trace(event);
	trace(car);
}
void sprite_select_mcga_backbuffer(void)
{
	trace(13);
}
void sprite_select_render_window(void)
{
	trace(14);
}
void set_projection(legacy_s16 x, legacy_s16 y, legacy_s16 width, legacy_s16 height)
{
	trace(15);
	trace(x);
	trace(y);
	trace(width);
	trace(height);
}
void sprite_set_target_clip_bounds(legacy_u16 left, legacy_u16 right, legacy_u16 top,
								   legacy_u16 bottom)
{
	trace(16);
	trace(left);
	trace(right);
	trace(top);
	trace(bottom);
}
void setup_car_shapes(legacy_s16 operation)
{
	trace(17);
	trace(operation);
}
void loop_game(legacy_s16 operation, legacy_s16 recorded, legacy_s16 current)
{
#ifdef RESTUNTS_SDL3
	if (scheduled_mode != 0) {
		if (operation == REPLAY_LOOP_HANDLE_INPUT) {
			keys++;
		}
		return;
	}
#endif
	assert(scripted_rewind == 0);
	trace(18);
	trace(operation);
	trace(recorded);
	trace(current);
}
void update_frame(legacy_s8 buffer, struct RECTANGLE *rect)
{
#ifdef RESTUNTS_SDL3
	if (scheduled_mode != 0) {
		frames++;
		return;
	}
#endif
	trace(19);
	trace(buffer);
	trace_rect(rect);
	trace(state.game_frame);
	trace(gameconfig.game_recordedframes);
	assert(++frames <= 3);
	if (scripted_rewind != 0) {
		assert(replaybar_enabled == 0);
		assert(keys == 0);
		if (frames == 3) {
			assert(game_replay_mode == REPLAY_MODE_LIVE);
			race_exit_request = REPLAY_EXIT_REQUESTED;
		} else {
			assert(game_replay_mode == REPLAY_MODE_PLAYBACK);
		}
		return;
	}
	elapsed_time2 = ++state.game_frame;
	if (frames == 3) {
		race_exit_request = REPLAY_EXIT_REQUESTED;
	}
}
void rect_union(struct RECTANGLE *first, struct RECTANGLE *second, struct RECTANGLE *result)
{
	assert(first == result);
	trace(20);
	trace_rect(first);
	trace_rect(second);
}
void shape2d_render_bmp_as_mask(struct SHAPE2D far *shape)
{
	(void)shape;
	trace(21);
}
void shape2d_rle_or_far_pointer(legacy_u16 offset, legacy_u16 segment)
{
	trace(22);
	trace(offset);
	trace(segment);
}
void frame_present(struct RECTANGLE *rect)
{
	trace(23);
	trace_rect(rect);
}
void mouse_draw_opaque_check(void)
{
	trace(24);
}
void mouse_draw_transparent_check(void)
{
	trace(25);
}
void sprite_present_mcga_backbuffer(void)
{
	trace(26);
}
void init_game_state_with_frame_rate(legacy_u16 rate)
{
	trace(27);
	trace(rate);
}
void mouse_minmax_position(legacy_s16 inset)
{
	trace(28);
	trace(inset);
}
void audio_carstate(void)
{
	trace(29);
}
legacy_s16 dos_kb_get_char(void)
{
#ifdef RESTUNTS_SDL3
	if (scheduled_mode != 0) {
		keys++;
		if (scheduled_toggle != 0 && state.game_frame == framespersec / 2) {
			supersight_enabled = 1;
		}
		return 0;
	}
#endif
	trace(30);
	keys++;
	if (idle_expired) {
		return scenario & 1 ? 27 : 0;
	}
	return keys % 3 == 1 ? KEY_LEFT : 0;
}
legacy_s16 handle_ingame_kb_shortcuts(legacy_s16 key)
{
	trace(31);
	trace(key);
	return 0;
}
void dos_mouse_get_state(legacy_s16 *buttons, legacy_s16 *x, legacy_s16 *y)
{
	trace(32);
	*buttons = scenario & 1;
	*x = 90;
	*y = 150;
}
legacy_s16 get_kb_or_joy_flags(void)
{
	trace(33);
	return scenario & 2 ? INPUT_ACTION_BUTTON_MASK : 0;
}

static void test_rewind_frame_loop(void)
{
	struct RACE_VIEWPORT_CACHE cache = {-1, -1};
	memset(&state, 0, sizeof(state));
	memset(&gameconfig, 0, sizeof(gameconfig));
	scripted_rewind = 1;
	frames = keys = 0;
	state.game_frame = elapsed_time2 = 10;
	state.game_inputmode = GAME_INPUT_MODE_ACTIVE;
	state.game_frames_per_sec = 40;
	gameconfig.game_recordedframes = 10;
	game_replay_mode = REPLAY_MODE_LIVE;
	game_replay_mode_copy = -1;
	race_exit_request = 0;
	race_start_sequence_state = RACE_START_SEQUENCE_INACTIVE;
	idle_expired = video_uses_page_flipping = recording_limit_warning_requested = 0;
	slow_video_mgmt_copy = slow_video_mgmt = 0;
	frame_buffer_index = dashboard_buffer_index = 0;
	dashb_toggle = is_in_replay = followOpponentFlag = 0;
	replaybar_toggle = 1;
	full_redraw_frames_remaining = 1;
	height_above_replaybar = 200;
	viewport_bottom_cache = -1;
	timer_ticks_per_frame = 5;
	race_run_frames(&cache);
	assert(frames == 3 && keys == 0);
	assert(state.game_frame == 7 && elapsed_time2 == 7);
	assert(gameconfig.game_recordedframes == 7);
	assert(game_replay_mode == REPLAY_MODE_LIVE && is_in_replay == 0);
}

#ifdef RESTUNTS_SDL3
static void prepare_presentation_test(legacy_u16 rate, legacy_u16 mode)
{
	memset(&state, 0, sizeof(state));
	memset(&gameconfig, 0, sizeof(gameconfig));
	scheduled_mode = 1;
	scheduled_toggle = mode == 2;
	scheduled_time = 0;
	scheduled_physics = scheduled_ghosts = scheduled_predictions = 0;
	scheduled_farthest_prediction = 0;
	scripted_rewind = 0;
	supersight_enabled = mode == 1;
	frames = keys = 0;
	state.game_inputmode = GAME_INPUT_MODE_ACTIVE;
	state.game_frames_per_sec = framespersec = configured_frame_rate = rate;
	game_replay_mode = REPLAY_MODE_LIVE;
	game_replay_mode_copy = -1;
	replay_playback_speed = REPLAY_PLAYBACK_NORMAL;
	race_start_sequence_state = RACE_START_SEQUENCE_INACTIVE;
	elapsed_time1 = elapsed_time2 = race_exit_request = 0;
	idle_expired = video_uses_page_flipping = recording_limit_warning_requested = 0;
	slow_video_mgmt = slow_video_mgmt_copy = 0;
	mouse_driving_enabled = 0;
	frame_buffer_index = dashboard_buffer_index = 0;
	dashb_toggle = is_in_replay = followOpponentFlag = replaybar_toggle = 0;
	full_redraw_frames_remaining = 1;
	height_above_replaybar = 200;
	viewport_bottom_cache = -1;
}

static void test_presentation_rate(void)
{
	for (legacy_u16 rate = 10; rate <= 20; rate += 10) {
		struct GAMESTATE baseline;
		for (legacy_u16 mode = 0; mode < 3; mode++) {
			struct RACE_VIEWPORT_CACHE cache = {-1, -1};
			prepare_presentation_test(rate, mode);
			race_run_frames(&cache);
			assert(scheduled_physics == rate);
			assert(keys == rate);
			assert(scheduled_ghosts == rate + 1U);
			assert((legacy_u16)state.game_frame == rate);
			if (mode == 0) {
				baseline = state;
				assert(frames == rate + 1U && scheduled_predictions == 0);
			} else {
				assert(memcmp(&baseline, &state, sizeof(state)) == 0);
				assert(scheduled_predictions != 0);
				assert(mode == 1 ? frames == 61U : frames > rate + 1U && frames < 61U);
			}
		}
	}
}

static void test_replay_presentation_rate(void)
{
	static const legacy_s16 speeds[] = {REPLAY_PLAYBACK_NORMAL, REPLAY_PLAYBACK_SLOW,
										REPLAY_PLAYBACK_FAST};
	for (legacy_u16 rate = 10; rate <= 20; rate += 10) {
		for (legacy_u16 speed = 0; speed < 3; speed++) {
			struct RACE_VIEWPORT_CACHE cache = {-1, -1};
			prepare_presentation_test(rate, 1);
			game_replay_mode = REPLAY_MODE_PLAYBACK;
			replay_playback_speed = speeds[speed];
			race_run_frames(&cache);
			legacy_u16 control_samples = speed == 1 ? rate / 2 : rate;
			legacy_u16 physics_steps = speed == 2 ? rate * 2 : control_samples;
			assert(scheduled_physics == physics_steps);
			assert(keys == control_samples);
			assert(scheduled_ghosts == control_samples + 1U);
			assert((legacy_u16)state.game_frame == physics_steps);
			assert(state.playerstate.car_position.lx == physics_steps * 60L);
			assert(frames == 61U);
			/* Fast replay predicts the full two-tick interval without freezing
			 * halfway to the next authoritative controller sample. */
			assert(scheduled_farthest_prediction > (speed == 2 ? 60U : 30U));
		}
	}
}

static void test_ghost_prediction_rate(void)
{
	race_presentation = (struct RACE_PRESENTATION){0};
	race_presentation.sample_span = 1;
	scheduled_ghost_active = 1;
	scheduled_ghost = (struct CARSTATE){0};
	scheduled_ghost_camera = (struct GHOST_CAMERA_STATE){0};
	state.game_frame = 0;
	race_presentation_capture_ghost();
	state.game_frame = 2;
	scheduled_ghost_camera.frame = 1;
	scheduled_ghost.car_position.lx = 100;
	race_presentation_capture_ghost();
	race_presentation_predict_ghost(FRAME_PREDICTION_ONE / 2U);
	assert(race_presentation.ghost_predicted.car_position.lx == 125);
	state.game_frame = 3;
	race_presentation_capture_ghost();
	race_presentation_predict_ghost(0);
	assert(race_presentation.ghost_predicted.car_position.lx == 150);
	race_presentation_predict_ghost(FRAME_PREDICTION_ONE / 2U);
	assert(race_presentation.ghost_predicted.car_position.lx == 175);
	state.game_frame = 4;
	scheduled_ghost_camera.frame = 2;
	scheduled_ghost.car_position.lx = 200;
	race_presentation_capture_ghost();
	race_presentation_predict_ghost(0);
	assert(race_presentation.ghost_predicted.car_position.lx == 200);
	state.game_frame = 6;
	race_presentation_capture_ghost();
	race_presentation_predict_ghost(FRAME_PREDICTION_ONE / 2U);
	assert(race_presentation.ghost_predicted.car_position.lx == 200);
	assert(scheduled_ghost.car_position.lx == 200);

	race_presentation = (struct RACE_PRESENTATION){0};
	race_presentation.sample_span = 2;
	state.game_frame = scheduled_ghost_camera.frame = 0;
	scheduled_ghost.car_position.lx = 0;
	race_presentation_capture_ghost();
	state.game_frame = scheduled_ghost_camera.frame = 2;
	scheduled_ghost.car_position.lx = 200;
	race_presentation_capture_ghost();
	race_presentation_predict_ghost(FRAME_PREDICTION_ONE / 2U);
	assert(race_presentation.ghost_predicted.car_position.lx == 300);
	assert(scheduled_ghost.car_position.lx == 200);
	scheduled_ghost_active = 0;
}

#endif

int main(void)
{
	struct RACE_VIEWPORT_CACHE cache;
	for (scenario = 0; scenario < 192; scenario++) {
		trace(scenario);
		memset(&state, 0, sizeof(state));
		memset(&gameconfig, 0, sizeof(gameconfig));
		memset(&rect_windshield, 0, sizeof(rect_windshield));
		memset(&dirty_rect, 0, sizeof(dirty_rect));
		cache.roof_height = cache.dashboard_bottom = -1;
		frames = keys = 0;
		state.game_frame = 10;
		elapsed_time2 = (scenario & 1) ? 11 : 10;
		gameconfig.game_recordedframes = 40;
		state.game_inputmode = (scenario & 2) ? GAME_INPUT_MODE_ACTIVE : GAME_INPUT_MODE_WAITING;
		game_replay_mode = (scenario % 3) == 0	 ? REPLAY_MODE_LIVE
						   : (scenario % 3) == 1 ? REPLAY_MODE_PLAYBACK
												 : REPLAY_MODE_PAUSED;
		game_replay_mode_copy = -1;
		race_exit_request = scenario & 4 ? 1 : 0;
		race_start_sequence_state =
			scenario & 8 ? RACE_START_SEQUENCE_INACTIVE : RACE_START_SEQUENCE_FLAG_ANIMATION;
		idle_expired = scenario & 16;
		video_uses_page_flipping = scenario & 32;
		video_page_count = 2;
		slow_video_mgmt = scenario & 64;
		slow_video_mgmt_copy = -1;
		mouse_driving_enabled = scenario & 1;
		recording_limit_warning_requested = scenario & 128;
		frame_buffer_index = dashboard_buffer_index = 0;
		dashb_toggle = scenario & 2;
		replaybar_toggle = scenario & 4;
		is_in_replay = scenario & 8;
		followOpponentFlag = scenario & 1;
		dashbmp_y = 140;
		roofbmpheight = 12;
		dastbmp_y = 130;
		full_redraw_frames_remaining = 1;
		height_above_replaybar = 200;
		viewport_bottom_cache = -1;
		active_frame_rects = &dirty_rect;
		configured_frame_rate = 20;
		race_run_frames(&cache);
		trace(frames);
		trace(keys);
		trace(game_replay_mode);
		trace(race_exit_request);
		trace(state.game_frame);
		trace(elapsed_time2);
		trace(gameconfig.game_recordedframes);
		trace(frame_buffer_index);
		trace(dashboard_buffer_index);
		trace(full_redraw_frames_remaining);
		trace(recording_limit_warning_requested);
		trace(race_start_sequence_state);
		trace(is_in_replay);
	}
#ifdef RACE_FRAMES_RECORD_BASELINE
	printf("Race frame fingerprint: %08" LEGACY_PRIx32 "\n", trace_hash);
#else
	assert(trace_hash == UINT32_C(0xe6335ceb));
#endif
	test_rewind_frame_loop();
#ifdef RESTUNTS_SDL3
	test_presentation_rate();
	test_replay_presentation_rate();
	test_ghost_prediction_rate();
#endif
	return 0;
}
