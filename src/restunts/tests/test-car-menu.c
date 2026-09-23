#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "../c/fileio.h"
#include "../c/legacy.h"
#include "../c/memmgr.h"
#include "../c/menu_internal.h"
#include "../c/platform.h"
#include "../c/shape2d.h"
#include "../c/shape3d.h"
#include "../c/ui_text.h"
#include "../c/timing.h"
#include "../c/game_input.h"
#include "../c/ui_input.h"
#include "../c/ui_dialog.h"
#include "../c/car_speed.h"
#include "../c/video_frame.h"
#include "../c/car_model.h"
#include "../c/scene_resources.h"
#include "../c/car_resources.h"
#include "../c/menu_common.h"
#include "../c/externs.h"
#include "../c/keyboard.h"
#ifdef RESTUNTS_SDL3
#include "../c/frame_internal.h"
#endif

#undef printf

struct SPRITE *render_window_sprite;

static legacy_u64 trace_hash = UINT64_C(1469598103934665603);
static legacy_u32 scenario, frame_index, file_index, allocation_index, sprite_index;
static legacy_u32 acceleration_step;
static legacy_u8 resource_bytes[64][32];
static struct SHAPE2D fixture_shapes[4];
static struct SPRITE fixture_sprites[4];
static const legacy_s8 *fixture_files[] = {(const legacy_s8 *)"CARVETT.RES",
										   (const legacy_s8 *)"CARANSX.RES",
										   (const legacy_s8 *)"CARCOUN.RES"};

#ifdef RESTUNTS_SDL3
legacy_u8 supersight_enabled;
legacy_u8 fps_display_enabled;
static legacy_u8 display_toggle_test, display_scenario, display_target, display_pending;
static legacy_u8 display_fps_drawn, display_previous_fps, display_refresh_pending;
static legacy_u32 display_present_count, display_fps_draw_count, display_record_count;
static legacy_u32 display_reset_count, display_shortcut_count, display_sprite_free_count;
static struct RECTANGLE display_clip, display_fps_bounds, display_previous_fps_bounds;

static void assert_contains(const struct RECTANGLE *outer, const struct RECTANGLE *inner)
{
	assert(outer->left <= inner->left && outer->right >= inner->right);
	assert(outer->top <= inner->top && outer->bottom >= inner->bottom);
}

static void record_preview_copy(void)
{
	if (display_toggle_test == 0 || display_pending == 0) {
		return;
	}
	assert(display_target == 0);
	assert(display_fps_drawn == fps_display_enabled);
	assert(display_reset_count != 0);
	if (display_fps_drawn != 0) {
		assert_contains(&display_clip, &display_fps_bounds);
	}
	if (display_previous_fps != 0) {
		assert_contains(&display_clip, &display_previous_fps_bounds);
	}
	display_previous_fps = display_fps_drawn;
	display_previous_fps_bounds = display_fps_bounds;
	display_fps_drawn = display_pending = display_refresh_pending = 0;
	display_present_count++;
	assert(display_record_count + 1U == display_present_count);
}

void frame_fps_reset(void)
{
	display_reset_count++;
}

void frame_fps_record_presented(void)
{
	if (display_toggle_test != 0) {
		display_record_count++;
		assert(display_record_count == display_present_count);
	}
}

struct RECTANGLE *frame_fps_draw_text(void)
{
	assert(fps_display_enabled != 0);
	if (display_toggle_test != 0) {
		assert(display_target == 1 && display_pending != 0);
		assert(display_fps_drawn == 0);
		display_fps_bounds.left = 8;
		display_fps_bounds.right = display_fps_draw_count % 2U == 0U ? 81 : 41;
		display_fps_bounds.top = 3;
		display_fps_bounds.bottom = 12;
		assert_contains(&display_clip, &display_fps_bounds);
		display_fps_drawn = 1;
		display_fps_draw_count++;
	}
	return &display_fps_bounds;
}
static legacy_u8 predictive_preview_test;
static legacy_u32 preview_present_count;
static legacy_s16 preview_last_rotation;
static legacy_u32 preview_physics_steps, preview_toggle_count;
static legacy_u64 preview_now, preview_previous_input_time;

legacy_u64 presentation_now(void)
{
	return preview_now;
}

legacy_s16 handle_ingame_kb_shortcuts(legacy_s16 key)
{
	assert(key == KEY_F11 || key == KEY_F12);
	if (key == KEY_F11) {
		fps_display_enabled ^= 1U;
		frame_fps_reset();
	} else {
		supersight_enabled ^= 1U;
	}
	display_shortcut_count++;
	display_refresh_pending = display_toggle_test;
	return 1;
}
#endif

static void trace_word(legacy_u16 value)
{
	trace_hash = (trace_hash ^ (value & 255U)) * UINT64_C(1099511628211);
	trace_hash = (trace_hash ^ (value >> 8)) * UINT64_C(1099511628211);
}

static void trace_text(const legacy_s8 *text)
{
	if (text == 0) {
		trace_word(65535U);
		return;
	}
	while (*text != 0) {
		trace_word((legacy_u8)*text++);
	}
	trace_word(0);
}

static void trace_pointer(const void *pointer)
{
	if (pointer == 0) {
		trace_word(0);
		return;
	}
	for (legacy_u32 i = 0; i < 64U; i++) {
		if (pointer == resource_bytes[i]) {
			trace_word(100U + i);
			return;
		}
	}
	for (legacy_u32 i = 0; i < 4U; i++) {
		if (pointer == &fixture_sprites[i]) {
			trace_word(200U + i);
			return;
		}
		if (pointer == &fixture_shapes[i]) {
			trace_word(300U + i);
			return;
		}
	}
	trace_word(1);
}

static void trace_rect(const struct RECTANGLE *rect)
{
	trace_word(rect->left);
	trace_word(rect->right);
	trace_word(rect->top);
	trace_word(rect->bottom);
}

legacy_s16 _strcmp(const legacy_s8 *dest, const legacy_s8 *src)
{
	while (*dest != 0 && *dest == *src) {
		dest++;
		src++;
	}
	return (legacy_s16)((legacy_u8)*dest - (legacy_u8)*src);
}

legacy_s8 *_strcpy(legacy_s8 *dest, const legacy_s8 *src)
{
	legacy_s8 *result = dest;
	do {
		*dest++ = *src;
	} while (*src++ != 0);
	return result;
}

void draw_button(legacy_s8 *text, legacy_s16 x, legacy_s16 y, legacy_s16 width, legacy_s16 height,
				 legacy_s16 top_color, legacy_s16 bottom_color, legacy_s16 fill_color,
				 legacy_s16 font_color)
{
	trace_word(1002);
	trace_text(text);
	trace_word((legacy_u16)x);
	trace_word((legacy_u16)y);
	trace_word((legacy_u16)width);
	trace_word((legacy_u16)height);
	trace_word((legacy_u16)top_color);
	trace_word((legacy_u16)bottom_color);
	trace_word((legacy_u16)fill_color);
	trace_word((legacy_u16)font_color);
}

void ensure_file_exists(legacy_s16 unused)
{
	trace_word(1003);
	trace_word((legacy_u16)unused);
}

const legacy_s8 *file_combine_and_find(const legacy_s8 *dir, const legacy_s8 *name,
									   const legacy_s8 *ext)
{
	trace_word(1004);
	trace_text(dir);
	trace_text(name);
	trace_text(ext);
	file_index = 0;
	if (scenario % 17U == 0U) {
		return 0;
	}
	return fixture_files[file_index++];
}

const legacy_s8 *file_find_next_alt(void)
{
	trace_word(1005);

	if (file_index < 3U) {
		return fixture_files[file_index++];
	}
	return 0;
}

void *file_load_resfile(const legacy_s8 *filename)
{
	trace_word(1006);
	trace_text(filename);
	assert(allocation_index < 64U);
	return resource_bytes[allocation_index++];
}

void *file_load_shape2d_fatal(const legacy_s8 *shapename)
{
	trace_word(1007);
	trace_text(shapename);
	assert(allocation_index < 64U);
	return resource_bytes[allocation_index++];
}

void font_draw_text(const legacy_s8 *text, legacy_s16 x, legacy_s16 y)
{
	trace_word(1008);
	trace_text(text);
	trace_word((legacy_u16)x);
	trace_word((legacy_u16)y);
}

void font_set_colors(legacy_s16 color, legacy_s16 background_color)
{
	trace_word(1009);
	trace_word((legacy_u16)color);
	trace_word((legacy_u16)background_color);
}

void font_set_fontdef(void)
{
	trace_word(1010);
}

void font_set_fontdef2(void *data)
{
	trace_word(1011);
	trace_pointer(data);
}

void init_game_state(legacy_s16 initialization_mode)
{
	trace_word(1012);
	trace_word((legacy_u16)initialization_mode);
	acceleration_step = 0;
	state.playerstate.car_rev_speed = 0;
}

legacy_s16 input_checking(legacy_s16 frame_delta)
{
	trace_word(1013);
	trace_word((legacy_u16)frame_delta);
#ifdef RESTUNTS_SDL3
	if (display_toggle_test != 0) {
		assert(display_refresh_pending == 0);
		static const legacy_u16 keys[] = {KEY_DOWN,	 KEY_DOWN, KEY_DOWN, KEY_F11, 0,		KEY_F12,
										  0,		 KEY_F11,  KEY_F11,	 0,		  KEY_F12,	KEY_F11,
										  KEY_ENTER, KEY_UP,   KEY_UP,	 KEY_UP,  KEY_ENTER};
		assert(frame_index < sizeof(keys) / sizeof(keys[0]));
		return keys[frame_index];
	}
	if (predictive_preview_test != 0) {
		if (supersight_enabled != 0) {
			assert(preview_now - preview_previous_input_time >= 10000000ULL);
		}
		preview_previous_input_time = preview_now;
		if (predictive_preview_test == 2 &&
			((preview_toggle_count == 0 && preview_now >= 250000000ULL) ||
			 (preview_toggle_count == 1 && preview_now >= 750000000ULL))) {
			preview_toggle_count++;
			return KEY_F12;
		}
		return preview_now >= 1000000000ULL ? KEY_ENTER : 0;
	}
#endif
	static const legacy_u16 keys[] = {0,		 KEY_DOWN, KEY_ENTER, KEY_ENTER, KEY_DOWN,
									  KEY_ENTER, KEY_DOWN, KEY_ENTER, KEY_DOWN,	 KEY_ENTER,
									  KEY_UP,	 KEY_UP,   KEY_UP,	  KEY_UP,	 KEY_ENTER};
	assert(frame_index < 100U);
	if (frame_index >= 15U) {
		return KEY_ENTER;
	}
	return keys[frame_index];
}

legacy_s8 *locate_shape_fatal(legacy_s8 *data, const legacy_s8 *name)
{
	trace_word(1014);
	trace_pointer(data);
	trace_text(name);
	return (legacy_s8 *)&fixture_shapes[0];
}

legacy_s8 *locate_text_res(legacy_s8 *data, const legacy_s8 *name)
{
	trace_word(1015);
	trace_pointer(data);
	trace_text(name);
	if (_strcmp(name, car_description_id) == 0) {
		return (legacy_s8 *)"First]Second]]Third]";
	}
	return (legacy_s8 *)name;
}

legacy_s16 menu_animate_button_highlight(legacy_s16 item_index, const struct BUTTON_AREA *buttons,
										 legacy_s16 second_color, legacy_s16 first_color)
{
	trace_word(1016);
	trace_word((legacy_u16)item_index);
	trace_pointer(buttons);
	trace_word((legacy_u16)second_color);
	trace_word((legacy_u16)first_color);
#ifdef RESTUNTS_SDL3
	if (predictive_preview_test != 0) {
		preview_now += 1000000ULL;
		return preview_now % 10000000ULL == 0;
	}
	preview_now += 20000000ULL;
#endif
	return (legacy_s16)(1U + frame_index % 3U);
}

void menu_reset_animation_timers(void)
{
	trace_word(1017);
}

void menu_update_idle_counter(legacy_u16 elapsed, legacy_s16 limit)
{
	trace_word(1018);
	trace_word((legacy_u16)elapsed);
	trace_word((legacy_u16)limit);
	if (scenario % 5U == 0U && frame_index >= 7U) {
		idle_expired = 1;
	}
}

void *mmgr_free(legacy_s8 *ptr)
{
	trace_word(1019);
	trace_pointer(ptr);
	return 0;
}

void mouse_draw_opaque_check(void)
{
	trace_word(1020);
}

void mouse_draw_transparent_check(void)
{
	trace_word(1021);
}

legacy_s16 mouse_multi_hittest(legacy_s16 count, const struct BUTTON_AREA *buttons)
{
	trace_word(1022);
	trace_word((legacy_u16)count);
#ifdef RESTUNTS_SDL3
	if (display_toggle_test != 0) {
		frame_index++;
		return -1;
	}
	if (predictive_preview_test != 0) {
		frame_index++;
		return 0;
	}
#endif
	trace_pointer(buttons);
	for (legacy_u32 i = 0; i < (legacy_u32)count; i++) {
		trace_word(buttons[i].x1);
		trace_word(buttons[i].x2);
		trace_word(buttons[i].y1);
		trace_word(buttons[i].y2);
	}
	legacy_s16 result = -1;
	if (frame_index == 10U && scenario % 3U == 0U) {
		result = 4;
	}
	if (frame_index >= 15U) {
		result = 0;
	}
	frame_index++;
	return result;
}

legacy_s16 polarAngle(legacy_s16 z, legacy_s16 y)
{
	trace_word(1023);
	trace_word((legacy_u16)z);
	trace_word((legacy_u16)y);
	return (legacy_s16)(z + y);
}

legacy_s16 rect_intersect(struct RECTANGLE *r1, struct RECTANGLE *r2)
{
	trace_word(1024);
	trace_rect(r1);
	trace_rect(r2);
#ifdef RESTUNTS_SDL3
	if (display_toggle_test != 0) {
		if (r1->left < r2->left) {
			r1->left = r2->left;
		}
		if (r1->right > r2->right) {
			r1->right = r2->right;
		}
		if (r1->top < r2->top) {
			r1->top = r2->top;
		}
		if (r1->bottom > r2->bottom) {
			r1->bottom = r2->bottom;
		}
		return 1;
	}
#endif
	r1->left = r2->left;
	r1->right = r2->right;
	r1->top = r2->top;
	r1->bottom = r2->bottom;
	return 1;
}

void rect_union(struct RECTANGLE *r1, struct RECTANGLE *r2, struct RECTANGLE *outrc)
{
	trace_word(1025);
	trace_rect(r1);
	trace_rect(r2);

	*outrc = *r1;
#ifdef RESTUNTS_SDL3
	if (display_toggle_test != 0) {
		if (r2->left < outrc->left) {
			outrc->left = r2->left;
		}
		if (r2->right > outrc->right) {
			outrc->right = r2->right;
		}
		if (r2->top < outrc->top) {
			outrc->top = r2->top;
		}
	}
#endif
	if (r2->bottom > outrc->bottom) {
		outrc->bottom = r2->bottom;
	}
}

legacy_u16 select_cliprect_rotate(legacy_s16 angZ, legacy_s16 angX, legacy_s16 angY,
								  struct RECTANGLE *cliprect, legacy_s16 half_scale)
{
	trace_word(1026);
	trace_word((legacy_u16)angZ);
	trace_word((legacy_u16)angX);
	trace_word((legacy_u16)angY);
	trace_rect(cliprect);
	trace_word((legacy_u16)half_scale);
	return 0;
}

void set_projection(legacy_s16 horizontal_fov_degrees, legacy_s16 vertical_fov_degrees,
					legacy_s16 width, legacy_s16 height)
{
	trace_word(1027);
	trace_word((legacy_u16)horizontal_fov_degrees);
	trace_word((legacy_u16)vertical_fov_degrees);
	trace_word((legacy_u16)width);
	trace_word((legacy_u16)height);
}

void setup_aero_trackdata(void *carresptr, legacy_s16 is_opponent)
{
	trace_word(1028);
	trace_pointer(carresptr);
	trace_word((legacy_u16)is_opponent);
}

legacy_u16 shape2d_get_height(const struct SHAPE2D *shape)
{
	trace_word(1029);
	trace_pointer(shape);
	return 12;
}

legacy_u16 shape2d_get_width(const struct SHAPE2D *shape)
{
	trace_word(1030);
	trace_pointer(shape);
	return 30;
}

void shape3d_free_car_shapes(void)
{
	trace_word(1031);
}

void shape3d_load_car_shapes(legacy_s8 *carid, legacy_s8 *opponent_carid)
{
	trace_word(1032);
	trace_text(carid);
	trace_text(opponent_carid);
}

void shape3d_render_queued_primitives(void)
{
#ifdef RESTUNTS_SDL3
	if (display_toggle_test != 0) {
		assert(display_pending == 0);
		display_pending = 1;
	}
	if (predictive_preview_test != 0) {
		preview_present_count++;
	}
#endif
	trace_word(1033);
}

legacy_u16 shape3d_transform_and_queue(struct TRANSFORMEDSHAPE3D *instance)
{
#ifdef RESTUNTS_SDL3
	if (predictive_preview_test != 0) {
		assert(instance->rotvec.z >= preview_last_rotation);
		preview_last_rotation = instance->rotvec.z;
	}
#endif
	trace_word(1034);
	trace_pointer(instance);
	trace_word(instance->rotvec.z);
	trace_word(instance->material);
	trace_word(instance->ts_flags);
	trace_word(instance->pos.x);
	trace_word(instance->pos.y);
	trace_word(instance->pos.z);
	if (instance->rectptr != 0) {
		instance->rectptr->left = 3;
		instance->rectptr->right = 128;
		instance->rectptr->top = 7;
		instance->rectptr->bottom = 88;
#ifdef RESTUNTS_SDL3
		if (display_toggle_test != 0) {
			/* Keep the car disjoint from the counter to expose missing FPS dirt. */
			instance->rectptr->left = 100;
			instance->rectptr->right = 180;
			instance->rectptr->top = 30;
			instance->rectptr->bottom = 80;
		}
#endif
	}
	return 0;
}

legacy_s16 sprite_blit_to_video(struct SPRITE *sprite, legacy_s16 mode)
{
	trace_word(1035);
	trace_pointer(sprite);
	trace_word((legacy_u16)mode);
#ifdef RESTUNTS_SDL3
	if (display_toggle_test != 0) {
		assert(sprite == render_window_sprite);
		record_preview_copy();
	}
#endif
	return 0;
}

void sprite_clear_shape_alt(struct SHAPE2D *shape, legacy_s16 x, legacy_s16 y)
{
	trace_word(1036);
	trace_pointer(shape);
	trace_word((legacy_u16)x);
	trace_word((legacy_u16)y);
}

void sprite_clear_target(legacy_u8 color)
{
	trace_word(1037);
	trace_word((legacy_u16)color);
}

void sprite_copy_image_at(struct SHAPE2D *shape, legacy_s16 x, legacy_s16 y)
{
	trace_word(1038);
	trace_pointer(shape);
	trace_word((legacy_u16)x);
	trace_word((legacy_u16)y);
}

void sprite_free_wnd(struct SPRITE *wndsprite)
{
#ifdef RESTUNTS_SDL3
	if (display_toggle_test != 0) {
		display_sprite_free_count++;
	}
#endif
	trace_word(1039);
	trace_pointer(wndsprite);
}

struct SPRITE *sprite_make_wnd(legacy_u16 width, legacy_u16 height, legacy_u16 color)
{
	trace_word(1040);
	trace_word((legacy_u16)width);
	trace_word((legacy_u16)height);
	trace_word((legacy_u16)color);
	assert(sprite_index < 4U);
	fixture_sprites[sprite_index].sprite_bitmapptr = &fixture_shapes[sprite_index];
	return &fixture_sprites[sprite_index++];
}

void sprite_putimage(struct SHAPE2D *shape)
{
#ifdef RESTUNTS_SDL3
	if (display_toggle_test != 0) {
		if (display_target == 1 && display_previous_fps != 0) {
			/* The background copy must erase old digits even after F11 turns off. */
			assert_contains(&display_clip, &display_previous_fps_bounds);
		} else if (display_target == 0) {
			assert(shape == render_window_sprite->sprite_bitmapptr);
			record_preview_copy();
		}
	}
#endif
	trace_word(1041);
	trace_pointer(shape);
}

void sprite_putimage_transparent(struct SHAPE2D *shape, legacy_s16 x, legacy_s16 y)
{
	trace_word(1042);
	trace_pointer(shape);
	trace_word((legacy_u16)x);
	trace_word((legacy_u16)y);
}

void sprite_putpixel_clipped(legacy_s16 x, legacy_s16 y, legacy_s16 color)
{
	trace_word(1043);
	trace_word((legacy_u16)x);
	trace_word((legacy_u16)y);
	trace_word((legacy_u16)color);
}

void sprite_select_mcga_backbuffer(void)
{
#ifdef RESTUNTS_SDL3
	display_target = 2;
#endif
	trace_word(1044);
}

void sprite_select_render_window(void)
{
#ifdef RESTUNTS_SDL3
	display_target = 1;
#endif
	trace_word(1045);
}

void sprite_select_render_window_and_clear(void)
{
#ifdef RESTUNTS_SDL3
	display_target = 1;
#endif
	trace_word(1046);
}

void sprite_select_screen_compat(void)
{
#ifdef RESTUNTS_SDL3
	display_target = 0;
#endif
	trace_word(1047);
}

void sprite_set_target_clip_bounds(legacy_u16 left, legacy_u16 right, legacy_u16 top,
								   legacy_u16 bottom)
{
#ifdef RESTUNTS_SDL3
	display_clip.left = left;
	display_clip.right = right;
	display_clip.top = top;
	display_clip.bottom = bottom;
#endif
	trace_word(1048);
	trace_word((legacy_u16)left);
	trace_word((legacy_u16)right);
	trace_word((legacy_u16)top);
	trace_word((legacy_u16)bottom);
}

void sprite_shape_to_1_alt(struct SHAPE2D *shape)
{
	trace_word(1049);
	trace_pointer(shape);
}

legacy_u32 timer_get_delta_alt(void)
{
	trace_word(1050);

	return 3;
}

void unload_resource(void *resptr)
{
	trace_word(1051);
	trace_pointer(resptr);
}

void update_car_speed(legacy_s8 input, legacy_s16 car_index, struct CARSTATE *carstate,
					  struct SIMD *simd)
{
	trace_word(1052);
	trace_word((legacy_u16)input);
	trace_word((legacy_u16)car_index);
	trace_pointer(carstate);
	trace_pointer(simd);
	trace_word(carstate->car_transmission);
	acceleration_step++;
#ifdef RESTUNTS_SDL3
	if (predictive_preview_test != 0) {
		preview_physics_steps++;
	}
#endif
	carstate->car_rev_speed =
		(legacy_s16)((scenario % 3U == 0U ? acceleration_step % 64U : acceleration_step) * 256U);
}

static void run_car_case(legacy_u32 index)
{
	legacy_s8 material = index % 6U;
	legacy_s8 transmission = index % 2U;
	scenario = index;
	frame_index = 0;
#ifdef RESTUNTS_SDL3
	preview_now = preview_previous_input_time = 0;
#endif
	file_index = 0;
	allocation_index = 0;
	sprite_index = 0;
	idle_expired = 0;
	video_uses_page_flipping = index % 2U;
	slow_video_mgmt = index % 3U == 0U;
#ifdef RESTUNTS_SDL3
	if (display_toggle_test != 0) {
		video_uses_page_flipping = display_scenario & 1U;
		slow_video_mgmt = (display_scenario >> 1) & 1U;
	}
#endif
	framespersec = 10 + index % 3U;
	miscptr = (legacy_s8 *)resource_bytes[63];
	fontnptr = (legacy_s8 *)resource_bytes[62];
	font_glyph_height = 8;
	game3dshapes[PLAYER_CAR_LOW_SHAPE].shape3d_numpaints = 3;
	for (legacy_u32 i = 0; i < 7U; i++) {
		oppresources[i] = (legacy_s8 *)&fixture_shapes[1];
	}
	trace_word(index);
	legacy_s8 car_id[5] = "COUN";
	legacy_u16 opponent_type = index % 3U == 0U ? 2U : 0U;
#ifdef RESTUNTS_SDL3
	if (display_toggle_test != 0) {
		opponent_type = (display_scenario & 4U) != 0 ? 2U : 0U;
	}
#endif
	run_car_menu(car_id, &material, &transmission, opponent_type);
#ifdef RESTUNTS_SDL3
	if (display_toggle_test != 0) {
		assert(_strcmp(car_id, (const legacy_s8 *)"COUN") == 0);
		assert(material == 1 && transmission == 0);
	}
#endif
	trace_text(car_id);
	trace_word((legacy_u8)material);
	trace_word((legacy_u8)transmission);
	trace_word(framespersec);
	trace_word(idle_expired);
	trace_word(waitflag);
}

#ifdef RESTUNTS_SDL3
static void test_display_toggles(void)
{
	predictive_preview_test = 0;
	display_toggle_test = 1;
	for (display_scenario = 0; display_scenario < 32; display_scenario++) {
		legacy_u8 initial_supersight = (display_scenario >> 3) & 1U;
		legacy_u8 initial_fps = (display_scenario >> 4) & 1U;
		supersight_enabled = initial_supersight;
		fps_display_enabled = initial_fps;
		display_pending = display_fps_drawn = display_previous_fps = display_refresh_pending = 0;
		display_present_count = display_fps_draw_count = display_record_count = 0;
		display_reset_count = display_shortcut_count = display_sprite_free_count = 0;
		run_car_case(1);
		assert(frame_index == 17);
		assert(supersight_enabled == initial_supersight && fps_display_enabled == initial_fps);
		assert(display_shortcut_count == 6);
		assert(display_reset_count == 6);
		assert(display_fps_draw_count > 0 && display_fps_draw_count < display_present_count);
		assert(display_present_count == display_record_count);
		assert(display_pending == 0);
		assert(display_sprite_free_count == sprite_index);
	}
	display_toggle_test = 0;
	puts("Car menu FPS toggles passed (32 scenarios).");
}
#endif

int main(void)
{
	for (legacy_u32 index = 0; index < 102U; index++) {
		run_car_case(index);
	}
	/* Original implementation trace: car discovery and sorting, car changes,
	 * graph rendering, animation phases, navigation, idle exit and resource cleanup. */
	assert(trace_hash == UINT64_C(0x25191d328ccaebd8));
#ifdef RESTUNTS_SDL3
	legacy_u32 reference_physics_steps = 0;
	legacy_u32 reference_polls = 0;
	for (legacy_u8 mode = 0; mode < 3; mode++) {
		supersight_enabled = mode == 1;
		predictive_preview_test = mode == 2 ? 2 : 1;
		preview_present_count = preview_physics_steps = preview_toggle_count = 0;
		preview_last_rotation = 0;
		run_car_case(1);
		assert(preview_last_rotation >= 97 && preview_last_rotation <= 100);
		if (mode == 0) {
			reference_physics_steps = preview_physics_steps;
			reference_polls = frame_index;
			assert(reference_polls == 1000);
		} else {
			assert(preview_physics_steps == reference_physics_steps);
			assert(frame_index <= reference_polls);
		}
		if (mode == 1) {
			assert(frame_index == 100);
			assert(preview_present_count == 40);
			/* The final 40 Hz presentation is at 975 ms of a 100-unit rotation. */
			assert(preview_last_rotation == 97);
		}
	}
	test_display_toggles();
#endif
	puts("Car menu interaction snapshots passed (102 scenarios).");
	return 0;
}
