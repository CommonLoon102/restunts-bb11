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

static struct {
	legacy_u8 active;
	legacy_u8 target;
	legacy_u16 masks[2];
	legacy_u16 overlays[2];
	legacy_u16 static_draws;
	legacy_u16 updates;
	legacy_u16 controls;
	struct RECTANGLE clip;
	struct SHAPE2D shapes[2];
} replay_render;

enum REPLAY_TEST_TARGET {
	REPLAY_TEST_RENDER_TARGET,
	REPLAY_TEST_PAGE_TARGET,
	REPLAY_TEST_SCREEN_TARGET
};

legacy_u8 supersight_enabled;

#ifdef RESTUNTS_SDL3
legacy_s16 camera_track_height_offset;
static legacy_u8 scheduled_mode;
static legacy_u8 scheduled_toggle;
static legacy_u64 scheduled_time;
static legacy_u32 scheduled_physics;
static legacy_u32 scheduled_ghosts;
static legacy_u32 scheduled_snapshots;
static legacy_u8 scheduled_ghost_active;
static legacy_u8 scheduled_ghost_sample_succeeds;
static legacy_u32 scheduled_ghost_sample_calls;
static legacy_u32 scheduled_ghost_sample_frame;
static legacy_u16 scheduled_ghost_sample_rate;
static legacy_u32 scheduled_ghost_sample_lag;
static struct CARSTATE scheduled_ghost;
static struct GHOST_CAMERA_STATE scheduled_ghost_camera;
static legacy_s8 scheduled_input[256];
static struct {
	legacy_u64 time;
	legacy_s32 position;
	legacy_s32 authoritative_position;
	legacy_s16 frame;
} scheduled_renders[128];

static void record_scheduled_frame(const struct GAMESTATE *rendered)
{
	assert(frames < sizeof(scheduled_renders) / sizeof(scheduled_renders[0]));
	scheduled_renders[frames].time = scheduled_time;
	scheduled_renders[frames].position = rendered->playerstate.car_position.lx;
	scheduled_renders[frames].authoritative_position = state.playerstate.car_position.lx;
	scheduled_renders[frames].frame = state.game_frame;
	frames++;
}

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

legacy_s16 ghost_sample_render_pose(legacy_u32 live_frame, legacy_u16 live_frame_rate,
									legacy_u32 lag, struct CARSTATE *car,
									struct GHOST_CAMERA_STATE *camera)
{
	scheduled_ghost_sample_calls++;
	scheduled_ghost_sample_frame = live_frame;
	scheduled_ghost_sample_rate = live_frame_rate;
	scheduled_ghost_sample_lag = lag;
	if (scheduled_ghost_sample_succeeds == 0) {
		return 0;
	}
	*car = scheduled_ghost;
	*camera = scheduled_ghost_camera;
	car->car_position.lx = live_frame * 100L - lag * 100L / FRAME_INTERPOLATION_ONE;
	camera->follow_position.x = (legacy_s16)car->car_position.lx;
	return 1;
}

void update_frame_snapshot(legacy_s8 buffer, struct RECTANGLE *rect,
						   const struct GAMESTATE *snapshot, const struct CARSTATE *ghost,
						   const struct GHOST_CAMERA_STATE *ghost_camera)
{
	(void)buffer;
	(void)rect;
	assert(scheduled_mode != 0 && supersight_enabled != 0);
	assert(ghost == NULL && ghost_camera == NULL);
	assert(snapshot->game_frame == state.game_frame);
	assert(snapshot->playerstate.car_position.lx <= state.playerstate.car_position.lx);
	if (race_presentation.history_valid != 0) {
		assert(snapshot->playerstate.car_position.lx >=
			   race_presentation.previous.playerstate.car_position.lx);
	}
	scheduled_snapshots++;
	record_scheduled_frame(snapshot);
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
	if (replay_render.active != 0) {
		return;
	}
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
	if (scan_code == RACE_CONTROL_SCAN_CODE) {
		return 0;
	}
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
	replay_render.target = REPLAY_TEST_PAGE_TARGET;
	trace(13);
}
void sprite_select_render_window(void)
{
	replay_render.target = REPLAY_TEST_RENDER_TARGET;
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
	replay_render.clip = (struct RECTANGLE){left, right, top, bottom};
	trace(16);
	trace(left);
	trace(right);
	trace(top);
	trace(bottom);
}
void setup_car_shapes(legacy_s16 operation)
{
	if (replay_render.active != 0) {
		if (operation == DASHBOARD_OPERATION_REDRAW_STATIC) {
			replay_render.static_draws++;
		} else {
			assert(operation == DASHBOARD_OPERATION_UPDATE);
			assert(replay_render.target == (video_uses_page_flipping != 0
												? REPLAY_TEST_PAGE_TARGET
												: REPLAY_TEST_SCREEN_TARGET));
			replay_render.updates++;
		}
	}
	trace(17);
	trace(operation);
}
void loop_game(legacy_s16 operation, legacy_s16 recorded, legacy_s16 current)
{
	if (replay_render.active != 0) {
		assert(operation == REPLAY_LOOP_DRAW_CONTROLS);
		replay_render.controls++;
		return;
	}
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
	if (replay_render.active != 0) {
		assert(rect == &rect_windshield);
		assert(rect->bottom == (supersight_enabled != 0 && replaybar_enabled != 0 ? 151 : 165));
		frames++;
		return;
	}
#ifdef RESTUNTS_SDL3
	if (scheduled_mode != 0) {
		record_scheduled_frame(&state);
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
static void record_dashboard_mask(legacy_u8 clipped, legacy_u8 overlay)
{
	if (replay_render.active == 0) {
		return;
	}
	assert(replay_render.target ==
		   (video_uses_page_flipping != 0 ? REPLAY_TEST_PAGE_TARGET : REPLAY_TEST_RENDER_TARGET));
	assert(clipped == (supersight_enabled != 0 && replaybar_enabled != 0));
	if (clipped != 0) {
		assert(replay_render.clip.left == 0 && replay_render.clip.right == 320);
		assert(replay_render.clip.top == 0 && replay_render.clip.bottom == 151);
	}
	if (overlay != 0) {
		replay_render.overlays[clipped]++;
	} else {
		replay_render.masks[clipped]++;
	}
}

void shape2d_render_bmp_as_mask(struct SHAPE2D far *shape)
{
	record_dashboard_mask(0, 0);
	(void)shape;
	trace(21);
}
void shape2d_rle_or_far_pointer(legacy_u16 offset, legacy_u16 segment)
{
	record_dashboard_mask(0, 1);
	trace(22);
	trace(offset);
	trace(segment);
}
void shape2d_rle_mask_position_clipped(struct SHAPE2D far *shape)
{
	record_dashboard_mask(1, 0);
	assert(shape == dasmshapeptr);
	assert(supersight_enabled != 0 && replaybar_enabled != 0);
}
void shape2d_rle_or_position_clipped(struct SHAPE2D far *shape)
{
	record_dashboard_mask(1, 1);
	assert(shape == (replay_render.active != 0 ? &replay_render.shapes[1] : NULL));
	assert(supersight_enabled != 0 && replaybar_enabled != 0);
}
void far *dos_memory_make_pointer(legacy_u16 segment, legacy_u16 offset)
{
	assert(segment == dastseg && offset == dastbmp_y2);
	return replay_render.active != 0 ? &replay_render.shapes[1] : NULL;
}
void frame_present(struct RECTANGLE *rect)
{
	if (replay_render.active != 0 && video_uses_page_flipping == 0) {
		replay_render.target = REPLAY_TEST_SCREEN_TARGET;
	}
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

static void test_replay_dashboard_rendering(void)
{
	legacy_u32 previous_trace = trace_hash;
	for (legacy_u16 mode = 0; mode < 16; mode++) {
		memset(&replay_render, 0, sizeof(replay_render));
		replay_render.active = 1;
		supersight_enabled = mode & 1;
		replaybar_toggle = (mode >> 1) & 1;
		video_uses_page_flipping = (mode >> 2) & 1;
		legacy_u8 full_redraw = (mode >> 3) & 1;
		struct RACE_VIEWPORT_CACHE cache = {-1, -1, 0};
		game_replay_mode = REPLAY_MODE_PLAYBACK;
		game_replay_mode_copy = RACE_REPLAY_MODE_UNINITIALIZED;
		idle_expired = followOpponentFlag = is_in_replay = 0;
		dashb_toggle = 1;
		dashbmp_y = 165;
		dastbmp_y = 130;
		roofbmpheight = 0;
		dasmshapeptr = &replay_render.shapes[0];
		viewport_bottom_cache = -1;
		video_page_count = video_uses_page_flipping != 0 ? 2 : 1;
		frame_buffer_index = dashboard_buffer_index = 0;
		slow_video_mgmt_copy = 0;
		frames = presented_frames = 0;
		race_update_viewport(&cache, 0);
		legacy_u8 clipped = supersight_enabled != 0 && replaybar_enabled != 0;
		assert(rect_windshield.bottom == (clipped != 0 ? 151 : 165));
		assert(dashboard_visible != 0);
		full_redraw_frames_remaining = full_redraw;
		if (video_uses_page_flipping != 0) {
			sprite_select_mcga_backbuffer();
		} else {
			sprite_select_render_window();
		}
		race_draw_frame();
		assert(replay_render.masks[clipped] == 1 && replay_render.overlays[clipped] == 1);
		assert(replay_render.masks[!clipped] == 0 && replay_render.overlays[!clipped] == 0);
		assert(replay_render.static_draws == full_redraw && replay_render.updates == 1);
		assert(replay_render.controls == (full_redraw != 0 && replaybar_enabled != 0));
		assert(replay_render.clip.left == 0 && replay_render.clip.right == 320);
		assert(replay_render.clip.top == 0 && replay_render.clip.bottom == 200);
		assert(frame_buffer_index == (video_uses_page_flipping != 0 ? 1 : 0));
		assert(full_redraw_frames_remaining == 0 && presented_frames == 1);
	}
	replay_render.active = 0;
	supersight_enabled = 0;
	assert(trace_hash == previous_trace);
}

static void test_rewind_frame_loop(void)
{
	struct RACE_VIEWPORT_CACHE cache = {-1, -1, 0};
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
	scheduled_physics = scheduled_ghosts = scheduled_snapshots = 0;
	scheduled_ghost_active = 0;
	scheduled_ghost_sample_succeeds = 1;
	scheduled_ghost_sample_calls = 0;
	memset(scheduled_renders, 0, sizeof(scheduled_renders));
	scripted_rewind = 0;
	supersight_enabled = mode == 1;
	frames = keys = presented_frames = 0;
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

static void assert_presentation_timestamps(legacy_u16 samples_per_second,
										   legacy_u16 distance_per_sample)
{
	assert(frames == 61U && scheduled_snapshots == frames);
	legacy_u64 interval = PRESENTATION_SECOND_NS / samples_per_second;
	const legacy_u16 slots_per_sample = PRESENTATION_RATE / samples_per_second;
	for (legacy_u32 index = 0; index < frames; index++) {
		legacy_u64 time = scheduled_renders[index].time;
		/* The frame loop sleeps in whole milliseconds. It must never present
		 * before the exact rational 60 Hz deadline. */
		assert(time == (index * 1000U + PRESENTATION_RATE - 1U) / PRESENTATION_RATE * 1000000ULL);
		legacy_u32 sample = (legacy_u32)(time / interval);
		legacy_s32 expected = 0;
		if (sample != 0) {
			/* Display evenly spaced points in the completed authoritative interval,
			 * ending at its newest state one visual slot before the next sample. */
			legacy_u32 fraction =
				(index % slots_per_sample + 1U) * FRAME_INTERPOLATION_ONE / slots_per_sample;
			expected = (sample - 1U) * distance_per_sample +
					   distance_per_sample * fraction / FRAME_INTERPOLATION_ONE;
		}
		assert(scheduled_renders[index].position == expected);
		assert(scheduled_renders[index].authoritative_position ==
			   (legacy_s32)sample * distance_per_sample);
		assert(scheduled_renders[index].frame * 60L ==
			   scheduled_renders[index].authoritative_position);
	}
}

static void test_presentation_rate(void)
{
	for (legacy_u16 rate = 10; rate <= 20; rate += 10) {
		struct GAMESTATE baseline;
		for (legacy_u16 mode = 0; mode < 3; mode++) {
			struct RACE_VIEWPORT_CACHE cache = {-1, -1, 0};
			prepare_presentation_test(rate, mode);
			race_run_frames(&cache);
			assert(scheduled_physics == rate);
			assert(keys == rate);
			assert(scheduled_ghosts == rate + 1U);
			assert((legacy_u16)state.game_frame == rate);
			if (mode == 0) {
				baseline = state;
				assert(frames == rate + 1U && scheduled_snapshots == 0);
			} else {
				assert(memcmp(&baseline, &state, sizeof(state)) == 0);
				assert(scheduled_snapshots != 0);
				if (mode == 1) {
					assert_presentation_timestamps(rate, 60);
				} else {
					assert(frames > rate + 1U && frames < 61U);
				}
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
			struct RACE_VIEWPORT_CACHE cache = {-1, -1, 0};
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
			assert_presentation_timestamps(control_samples, speed == 2 ? 120 : 60);
		}
	}
}

static void prepare_interpolation_pair(void)
{
	prepare_presentation_test(20, 1);
	race_presentation = (struct RACE_PRESENTATION){0};
	race_presentation_sync(0);
	scheduled_time = 50000000U;
	state.game_frame = elapsed_time2 = 1;
	state.playerstate.car_position.lx = 60;
	race_presentation_capture();
	assert(race_presentation.history_valid != 0);
	race_presentation.started = 1;
}

static void assert_visual_position(legacy_s32 position)
{
	struct GAMESTATE authoritative = state;
	legacy_u32 physics = scheduled_physics;
	legacy_u32 input_samples = keys;
	race_draw_visual();
	assert(scheduled_renders[frames - 1U].position == position);
	assert(memcmp(&authoritative, &state, sizeof(state)) == 0);
	assert(scheduled_physics == physics && keys == input_samples);
}

static void test_interpolation_slot_boundaries(void)
{
	for (legacy_u16 rate = 10; rate <= 20; rate += 10) {
		for (legacy_u16 slow = 0; slow < 2; slow++) {
			prepare_interpolation_pair();
			framespersec = rate;
			game_replay_mode = REPLAY_MODE_PLAYBACK;
			replay_playback_speed = slow != 0 ? REPLAY_PLAYBACK_SLOW : REPLAY_PLAYBACK_NORMAL;
			legacy_u16 slots = PRESENTATION_RATE / rate * (slow != 0 ? 2U : 1U);
			legacy_u64 origin = scheduled_time;
			struct GAMESTATE authoritative = state;
			race_present_interpolation();
			assert(frames == 1);
			assert(race_presentation_fraction(scheduled_time) == FRAME_INTERPOLATION_ONE / slots);
			for (legacy_u16 slot = 1; slot < slots; slot++) {
				legacy_u64 deadline =
					origin + ((legacy_u64)slot * PRESENTATION_SECOND_NS + PRESENTATION_RATE - 1U) /
								 PRESENTATION_RATE;
				scheduled_time = deadline - 1U;
				assert(race_presentation_fraction(scheduled_time) ==
					   slot * FRAME_INTERPOLATION_ONE / slots);
				race_present_interpolation();
				assert(frames == slot);
				scheduled_time = deadline;
				legacy_u32 fraction = (slot + 1U) * FRAME_INTERPOLATION_ONE / slots;
				assert(race_presentation_fraction(scheduled_time) == fraction);
				race_present_interpolation();
				assert(frames == slot + 1U);
				assert(scheduled_renders[slot].position ==
					   (legacy_s32)(60U * fraction / FRAME_INTERPOLATION_ONE));
			}
			assert(race_presentation_fraction(scheduled_time) == FRAME_INTERPOLATION_ONE);
			assert(scheduled_renders[frames - 1U].position == 60);
			assert(memcmp(&authoritative, &state, sizeof(state)) == 0);
			assert(scheduled_physics == 0 && keys == 0);
		}
	}
}

static void test_interpolation_slots_and_stalls(void)
{
	prepare_interpolation_pair();
	assert_visual_position(19);
	scheduled_time = 66666666U;
	assert_visual_position(19);
	scheduled_time = 66666667U;
	assert_visual_position(39);
	scheduled_time = 83333333U;
	assert_visual_position(39);
	scheduled_time = 83333334U;
	assert_visual_position(60);
	scheduled_time = 500000000U;
	assert_visual_position(60);

	prepare_interpolation_pair();
	race_present_interpolation();
	assert(frames == 1 && scheduled_renders[0].position == 19);
	/* A late frame skips missed slots without drawing a catch-up burst. */
	scheduled_time = 181000000U;
	race_present_interpolation();
	assert(frames == 2 && scheduled_renders[1].position == 60);
	race_present_interpolation();
	assert(frames == 2);
	scheduled_time = 183333333U;
	race_present_interpolation();
	assert(frames == 2);
	scheduled_time = 183333334U;
	race_present_interpolation();
	assert(frames == 3 && scheduled_renders[2].position == 60);
}

static void test_interpolation_resets(void)
{
	for (legacy_u16 reset = 0; reset < 6; reset++) {
		prepare_interpolation_pair();
		assert_visual_position(19);
		if (reset == 0) {
			cameramode++;
		} else if (reset == 1) {
			supersight_enabled = 0;
			race_presentation_sync(0);
			supersight_enabled = 1;
		} else if (reset == 2) {
			game_replay_mode = REPLAY_MODE_PLAYBACK;
			is_in_replay = 1;
		} else if (reset == 3) {
			custom_camera.distance++;
		} else if (reset == 4) {
			race_presentation_sync(1);
		} else {
			replay_playback_speed = REPLAY_PLAYBACK_FAST;
		}
		race_presentation_sync(0);
		assert(race_presentation.history_valid == 0);
		assert_visual_position(60);
	}
	for (legacy_u16 backwards = 0; backwards < 2; backwards++) {
		prepare_interpolation_pair();
		state.game_frame = elapsed_time2 = backwards != 0 ? 0 : 10;
		state.playerstate.car_position.lx = state.game_frame * 60L;
		race_presentation_capture();
		assert(race_presentation.history_valid == 0);
		assert_visual_position(state.playerstate.car_position.lx);
		state.game_frame = ++elapsed_time2;
		state.playerstate.car_position.lx += 60;
		scheduled_time += 50000000U;
		race_presentation_capture();
		assert(race_presentation.history_valid != 0);
		assert_visual_position(state.playerstate.car_position.lx - 41);
	}
}

static void test_ghost_interpolation_timestamps(void)
{
	prepare_interpolation_pair();
	scheduled_ghost_active = 1;
	scheduled_ghost = (struct CARSTATE){0};
	scheduled_ghost.car_position.lx = 500;
	scheduled_ghost_camera = (struct GHOST_CAMERA_STATE){0};
	scheduled_ghost_camera.follow_position.x = 500;
	elapsed_time1 = 7;
	race_presentation_interpolate_ghost(FRAME_INTERPOLATION_ONE / 2U);
	assert(race_presentation.ghost_valid != 0);
	assert(scheduled_ghost_sample_calls == 1);
	assert(scheduled_ghost_sample_frame == 8 && scheduled_ghost_sample_rate == 20);
	assert(scheduled_ghost_sample_lag == FRAME_INTERPOLATION_ONE / 2U);
	assert(race_presentation.ghost_interpolated.car_position.lx == 750);
	assert(race_presentation.ghost_camera_interpolated.follow_position.x == 750);
	race_presentation_interpolate_ghost(FRAME_INTERPOLATION_ONE);
	assert(scheduled_ghost_sample_lag == 0);
	assert(race_presentation.ghost_interpolated.car_position.lx == 800);

	/* Fast playback spans two live ticks; the same visual fraction must map
	 * to the same recorded time for the ghost as for the player's pose. */
	race_presentation.sample_span = 2;
	race_presentation_interpolate_ghost(FRAME_INTERPOLATION_ONE / 4U);
	assert(scheduled_ghost_sample_lag == FRAME_INTERPOLATION_ONE * 3U / 2U);
	assert(race_presentation.ghost_interpolated.car_position.lx == 650);

	scheduled_ghost_sample_succeeds = 0;
	race_presentation_interpolate_ghost(FRAME_INTERPOLATION_ONE / 2U);
	assert(race_presentation.ghost_valid != 0);
	assert(race_presentation.ghost_interpolated.car_position.lx == 500);
	assert(race_presentation.ghost_camera_interpolated.follow_position.x == 500);
	legacy_u32 reads = scheduled_ghost_sample_calls;
	race_presentation.history_valid = 0;
	scheduled_ghost_sample_succeeds = 1;
	race_presentation_interpolate_ghost(FRAME_INTERPOLATION_ONE / 2U);
	assert(scheduled_ghost_sample_calls == reads);
	assert(race_presentation.ghost_interpolated.car_position.lx == 500);
	assert(scheduled_ghost.car_position.lx == 500);
	assert(scheduled_ghost_camera.follow_position.x == 500);
	scheduled_ghost_active = 0;
	race_presentation_interpolate_ghost(FRAME_INTERPOLATION_ONE / 2U);
	assert(race_presentation.ghost_valid == 0);
	assert(scheduled_ghost_sample_calls == reads);
}

#endif

int main(void)
{
#ifdef RESTUNTS_SDL3
	replay_input_buffer = scheduled_input;
#endif
	struct RACE_VIEWPORT_CACHE cache;
	for (scenario = 0; scenario < 192; scenario++) {
		trace(scenario);
		memset(&state, 0, sizeof(state));
		memset(&gameconfig, 0, sizeof(gameconfig));
		memset(&rect_windshield, 0, sizeof(rect_windshield));
		memset(&dirty_rect, 0, sizeof(dirty_rect));
		cache.roof_height = cache.dashboard_bottom = -1;
		cache.supersight = 0;
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
	test_replay_dashboard_rendering();
#ifdef RESTUNTS_SDL3
	test_presentation_rate();
	test_replay_presentation_rate();
	test_interpolation_slot_boundaries();
	test_interpolation_slots_and_stalls();
	test_interpolation_resets();
	test_ghost_interpolation_timestamps();
#endif
	return 0;
}
