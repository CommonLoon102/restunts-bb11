#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../c/audio_control.h"
#include "../c/fileio.h"
#include "../c/keyboard.h"
#include "../c/memmgr.h"
#include "../c/menu_common.h"
#include "../c/menu_internal.h"
#include "../c/resource.h"
#include "../c/ui_dialog.h"
#include "../c/ui_input.h"

#define TEST_MENU_BUTTON_COUNT 5U
#define TEST_SCREEN_WIDTH 320U
#define TEST_SCREEN_HEIGHT 200U
#define TEST_TRANSPARENT_COLOR 15U
#define TEST_SLOW_OPERATION_TICKS 100U
#define TEST_FRAME_TICKS 3U
#define TEST_EXISTING_SONG_TICKS 37U
#define TEST_MOUSE_NONE (-1)
#define TEST_IDLE_LIMIT 6000

enum TEST_SELECTION { TEST_DRIVE = 0, TEST_TRACK = 3, TEST_CANCEL = -1 };

legacy_s8 run_menu(void);
legacy_s8 is_audioloaded;
legacy_s8 idle_expired;
legacy_s16 waitflag;
legacy_s16 menu_highlight_second_color;
legacy_s16 menu_highlight_first_color;
legacy_s8 main_menu_shapes_name[] = "menu";
legacy_s8 main_menu_background_data[] = "back";
struct BUTTON_AREA menu_buttons[TEST_MENU_BUTTON_COUNT];
struct SPRITE *render_window_sprite;

static struct SPRITE window;
static struct SHAPE2D background;
static legacy_s8 shape_resource, song_resource;
static legacy_u8 existing_song, missing_song_header, song_playing;
static legacy_u8 window_live, resource_live, background_drawn, screen_selected;
static legacy_u32 prepare_count, activate_count, present_count, reset_count;
static legacy_u32 song_ticks, frame_index, frame_count, idle_frame;
static const legacy_u16 *keys;
static const legacy_s16 *selections;

static void simulate_slow_operation(void)
{
	/* Loading and drawing can each take longer than the missing opening. A new
	 * song must remain stopped throughout them; an existing song must continue. */
	if (present_count == 0 && existing_song == 0) {
		assert(song_playing == 0);
		assert(song_ticks == 0);
	}
	if (song_playing != 0) {
		song_ticks += TEST_SLOW_OPERATION_TICKS;
	}
}

void *file_prepare_audiores(const legacy_s8 *songfile, const legacy_s8 *voicefile,
							const legacy_s8 *name)
{
	assert(memcmp(songfile, "skidslct", sizeof("skidslct")) == 0);
	assert(memcmp(voicefile, "skidms", sizeof("skidms")) == 0);
	assert(memcmp(name, "SLCT", sizeof("SLCT")) == 0);
	assert(existing_song == 0 && prepare_count == 0);
	/* The music must outlive the menu's temporary arena allocations. */
	assert(window_live == 0 && resource_live == 0);
	prepare_count++;
	is_audioloaded = 1;
	simulate_slow_operation();
	return missing_song_header != 0 ? NULL : &song_resource;
}

void load_audio_finalize(void *resource)
{
	assert(resource == (missing_song_header != 0 ? NULL : &song_resource));
	assert(prepare_count == 1 && activate_count == 0);
	assert(present_count == 1 && reset_count == 1 && screen_selected != 0);
	assert(resource_live == 0 && window_live != 0);
	assert(song_playing == 0 && song_ticks == 0);
	activate_count++;
	song_playing = 1;
}

void show_waiting(void)
{
	assert(existing_song != 0 || prepare_count == 1);
	simulate_slow_operation();
}

struct SPRITE *sprite_make_wnd(legacy_u16 width, legacy_u16 height, legacy_u16 color)
{
	assert(width == TEST_SCREEN_WIDTH && height == TEST_SCREEN_HEIGHT);
	assert(color == TEST_TRANSPARENT_COLOR);
	assert(existing_song != 0 || prepare_count == 1);
	assert(window_live == 0);
	window_live = 1;
	simulate_slow_operation();
	return &window;
}

void *file_load_resource(legacy_s16 resource_type, const legacy_s8 *filename)
{
	assert(resource_type == FILE_RESOURCE_SHAPE2D && filename == main_menu_shapes_name);
	assert(window_live != 0 && resource_live == 0);
	resource_live = 1;
	simulate_slow_operation();
	return &shape_resource;
}

legacy_s8 *locate_shape_fatal(legacy_s8 *data, const legacy_s8 *name)
{
	assert(data == &shape_resource && name == main_menu_background_data);
	assert(resource_live != 0);
	return (legacy_s8 *)&background;
}

void sprite_select_render_window(void)
{
	assert(window_live != 0);
	screen_selected = 0;
}

void sprite_shape_to_1_alt(struct SHAPE2D *shape)
{
	assert(shape == &background && screen_selected == 0 && resource_live != 0);
	simulate_slow_operation();
	background_drawn = 1;
}

void *mmgr_free(legacy_s8 *resource)
{
	assert(resource == &shape_resource && resource_live != 0 && background_drawn != 0);
	resource_live = 0;
	return NULL;
}

legacy_s16 sprite_blit_to_video(struct SPRITE *sprite, legacy_s16 mode)
{
	assert(sprite == &window && window_live != 0 && background_drawn != 0);
	assert(resource_live == 0 && screen_selected == 0);
	assert((legacy_u8)mode ==
		   (present_count == 0 ? MENU_BLIT_MODE_INITIAL : MENU_BLIT_MODE_REFRESH));
	simulate_slow_operation();
	present_count++;
	return 0;
}

void sprite_select_screen_compat(void)
{
	assert(present_count != 0);
	screen_selected = 1;
}

void menu_reset_animation_timers(void)
{
	assert(screen_selected != 0);
	reset_count++;
}

legacy_s16 menu_animate_button_highlight(legacy_s16 item_index, const struct BUTTON_AREA *buttons,
										 legacy_s16 second_color, legacy_s16 first_color)
{
	assert(frame_index < frame_count && item_index == selections[frame_index]);
	assert(buttons == menu_buttons && second_color == menu_highlight_second_color);
	assert(first_color == menu_highlight_first_color);
	assert(song_playing != 0);
	assert(activate_count == (existing_song != 0 ? 0U : 1U));
	if (frame_index == 0 && existing_song == 0) {
		assert(song_ticks == 0);
	}
	song_ticks += TEST_FRAME_TICKS;
	return TEST_FRAME_TICKS;
}

legacy_s16 input_checking(legacy_s16 frame_delta)
{
	assert(frame_delta == TEST_FRAME_TICKS && frame_index < frame_count);
	return LEGACY_S16_FROM_BITS(keys[frame_index]);
}

legacy_s16 mouse_multi_hittest(legacy_s16 count, const struct BUTTON_AREA *buttons)
{
	assert(count == TEST_MENU_BUTTON_COUNT && buttons == menu_buttons);
	frame_index++;
	return TEST_MOUSE_NONE;
}

void menu_update_idle_counter(legacy_u16 elapsed, legacy_s16 limit)
{
	assert(elapsed == TEST_FRAME_TICKS && limit == TEST_IDLE_LIMIT);
	idle_expired = frame_index == idle_frame;
}

void sprite_free_wnd(struct SPRITE *sprite)
{
	assert(sprite == &window && window_live != 0 && resource_live == 0);
	assert(song_playing != 0);
	window_live = 0;
}

static void check_menu(const legacy_u16 *input_keys, const legacy_s16 *expected_selections,
					   legacy_u32 input_count, legacy_s8 expected_result, legacy_u8 already_playing,
					   legacy_u8 missing_header, legacy_u32 expire_frame)
{
	keys = input_keys;
	selections = expected_selections;
	frame_count = input_count;
	idle_frame = expire_frame;
	existing_song = already_playing;
	missing_song_header = missing_header;
	is_audioloaded = already_playing;
	song_playing = already_playing;
	song_ticks = already_playing != 0 ? TEST_EXISTING_SONG_TICKS : 0;
	window_live = resource_live = background_drawn = screen_selected = 0;
	prepare_count = activate_count = present_count = reset_count = frame_index = 0;
	idle_expired = 0;

	assert(run_menu() == expected_result);
	assert(frame_index == input_count && window_live == 0 && resource_live == 0);
	assert(prepare_count == (already_playing != 0 ? 0U : 1U));
	assert(activate_count == prepare_count);
	assert(song_playing != 0 && is_audioloaded != 0);
	assert(song_ticks >= input_count * TEST_FRAME_TICKS);
	if (already_playing != 0) {
		assert(song_ticks > TEST_EXISTING_SONG_TICKS);
	}
}

legacy_int main(void)
{
	static const legacy_u16 enter_keys[] = {KEY_ENTER};
	static const legacy_s16 enter_selections[] = {TEST_DRIVE};
	static const legacy_u16 redraw_keys[] = {KEY_RIGHT, 0, KEY_SPACE};
	static const legacy_s16 redraw_selections[] = {TEST_DRIVE, TEST_TRACK, TEST_TRACK};
	static const legacy_u16 cancel_keys[] = {KEY_ESCAPE};
	static const legacy_u16 idle_keys[] = {KEY_RIGHT, 0};
	static const legacy_s16 idle_selections[] = {TEST_DRIVE, TEST_TRACK};
	const legacy_u32 redraw_count = sizeof(redraw_keys) / sizeof(redraw_keys[0]);
	const legacy_u32 idle_count = sizeof(idle_keys) / sizeof(idle_keys[0]);

	check_menu(enter_keys, enter_selections, 1, TEST_DRIVE, 0, 0, 0);
	check_menu(redraw_keys, redraw_selections, redraw_count, TEST_TRACK, 0, 0, 0);
	assert(present_count == 2 && reset_count == present_count);
	check_menu(redraw_keys, redraw_selections, redraw_count, TEST_TRACK, 1, 0, 0);
	assert(present_count == 2 && reset_count == present_count);
	check_menu(cancel_keys, enter_selections, 1, TEST_CANCEL, 0, 0, 0);
	check_menu(idle_keys, idle_selections, idle_count, TEST_DRIVE, 0, 0, idle_count);
	check_menu(enter_keys, enter_selections, 1, TEST_DRIVE, 0, 1, 0);
	puts("Main menu audio start and lifetime tests passed.");
	return 0;
}
