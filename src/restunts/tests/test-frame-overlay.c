#include <assert.h>
#include <string.h>

#include "../c/frame_internal.h"
#include "../c/externs.h"
#include "../c/shape3d.h"
#include "../c/shape3d_internal.h"
#include "../c/shape2d.h"
#include "../c/shape2d_internal.h"
#include "../c/residue.h"
#include "../c/platform.h"
#include "../c/ui_text.h"
#include "../c/memmgr.h"
#include "../c/camera.h"
#include "../c/video_frame.h"
#include "../c/fatal.h"
#include "../c/game_input.h"
#include "../c/dashboard.h"

#undef strcmp
#undef memcpy
#undef strlen

struct LEGACY_EXECUTION_RESIDUE legacy_execution_residue;
legacy_s16 legacy_render_player_headings_active;
legacy_u8 fps_display_enabled;
static legacy_u32 realtime_ticks;

legacy_u32 dos_timer_get_realtime_counter(void)
{
	return realtime_ticks;
}

static legacy_u8 crack_lines[16];
static legacy_u8 crack_info[6];
static legacy_u16 crack_offset;
static legacy_u16 last_line[DRAW_LINE_WORD_COUNT];
static unsigned drawn_lines;
static legacy_s16 player[LEGACY_RESIDUE_WORD_COUNT];
static legacy_s16 opponent[LEGACY_RESIDUE_WORD_COUNT];

struct TEXT_DRAW {
	char text[REPLAY_FILENAME_SIZE];
	legacy_s16 x;
	legacy_s16 y;
	legacy_s16 color;
	legacy_s16 shadow_color;
};

static struct TEXT_DRAW text_draws[6];
static unsigned text_draw_count;
static struct RECTANGLE text_bounds;
static struct RECTANGLE restored_roof_bounds;
static struct RECTANGLE copied_roof_bounds;
static unsigned restored_roof_count;
static unsigned copied_roof_count;
static struct SHAPE2D roof_shape;
static struct SHAPE2D frame_shape;
static struct SPRITE frame_sprite;
struct SPRITE far *render_window_sprite;
legacy_s8 dashboard_roof_shape_id[] = "roof";

void sprite_save_context(struct SPRITE context[SPRITE_STATE_COUNT])
{
	context[0] = drawing_sprite;
	context[1] = screen_sprite;
}

void sprite_restore_context(struct SPRITE context[SPRITE_STATE_COUNT])
{
	drawing_sprite = context[0];
	screen_sprite = context[1];
}

void sprite_set_target_clip_bounds(legacy_u16 left, legacy_u16 right, legacy_u16 top,
								   legacy_u16 bottom)
{
	drawing_sprite.sprite_raster_left = left;
	drawing_sprite.sprite_raster_right = right;
	drawing_sprite.sprite_top = top;
	drawing_sprite.sprite_bottom = bottom;
}

legacy_s8 far *locate_shape_nofatal(legacy_s8 far *resource, const legacy_s8 *name)
{
	assert(resource == stdaresptr);
	assert(name == dashboard_roof_shape_id);
	return (legacy_s8 far *)&roof_shape;
}

void shape2d_rle_copy_position_clipped(struct SHAPE2D far *shape)
{
	assert(shape == &roof_shape);
	restored_roof_bounds.left = drawing_sprite.sprite_raster_left;
	restored_roof_bounds.right = drawing_sprite.sprite_raster_right;
	restored_roof_bounds.top = drawing_sprite.sprite_top;
	restored_roof_bounds.bottom = drawing_sprite.sprite_bottom;
	restored_roof_count++;
}

void sprite_putimage(struct SHAPE2D far *shape)
{
	assert(shape == &frame_shape);
	copied_roof_bounds.left = drawing_sprite.sprite_raster_left;
	copied_roof_bounds.right = drawing_sprite.sprite_raster_right;
	copied_roof_bounds.top = drawing_sprite.sprite_top;
	copied_roof_bounds.bottom = drawing_sprite.sprite_bottom;
	copied_roof_count++;
}

void mouse_draw_opaque_check(void)
{
}

void mouse_draw_transparent_check(void)
{
}

void fatal_error(const legacy_s8 *format, ...)
{
	(void)format;
	assert(0 && "Unexpected fatal error");
}

legacy_s8 far *locate_text_res(legacy_s8 far *resource, const legacy_s8 *name)
{
	assert(resource == gameresptr);
	if (strcmp((const char *)name, "rpl") == 0) {
		return (legacy_s8 *)"Replay";
	}
	if (strcmp((const char *)name, "dm1") == 0) {
		return (legacy_s8 *)"Demo";
	}
	assert(strcmp((const char *)name, "dm2") == 0);
	return (legacy_s8 *)"Press a key";
}

struct RECTANGLE *intro_draw_text(legacy_s8 *text, legacy_s16 x, legacy_s16 y, legacy_s16 color,
								  legacy_s16 shadow_color)
{
	size_t length = strlen((const char *)text);
	assert(text_draw_count < sizeof(text_draws) / sizeof(text_draws[0]));
	struct TEXT_DRAW *draw = &text_draws[text_draw_count++];
	assert(length < sizeof(draw->text));
	memcpy(draw->text, text, length + 1);
	draw->x = x;
	draw->y = y;
	draw->color = color;
	draw->shadow_color = shadow_color;
	/* Match the 8-pixel font and the shadow's extra pixel in the real renderer. */
	text_bounds.left = x;
	text_bounds.right = x + length * 8 + 1;
	text_bounds.top = y;
	text_bounds.bottom = y + 9;
	return &text_bounds;
}

legacy_s16 font_centered_text_x(const legacy_s8 *text)
{
	return (320 - strlen((const char *)text) * 8) / 2;
}

void format_frame_as_string(legacy_s8 *destination, legacy_s16 frame_count,
							legacy_s16 include_hundredths)
{
	(void)destination;
	(void)frame_count;
	(void)include_hundredths;
	assert(0 && "Unexpected penalty text");
}

void sprite_putpixel_clipped(legacy_s16 x, legacy_s16 y, legacy_s16 color)
{
	(void)x;
	(void)y;
	(void)color;
	assert(0 && "Unexpected ghost pixel");
}

void sprite_putimage_transparent(struct SHAPE2D far *shape, legacy_s16 x, legacy_s16 y)
{
	(void)shape;
	(void)x;
	(void)y;
	assert(0 && "Unexpected route icon");
}

legacy_s8 far *locate_shape_alt(legacy_s8 far *resource, const legacy_s8 *name)
{
	assert(resource == gameresptr);
	if (strcmp((const char *)name, "crak") == 0) {
		return (legacy_s8 far *)crack_lines;
	}
	assert(strcmp((const char *)name, "cinf") == 0);
	return (legacy_s8 far *)crack_info;
}

legacy_u16 dos_memory_pointer_offset(const void far *pointer)
{
	assert(pointer == crack_lines);
	return crack_offset;
}

void sprite_draw_line_from_setup(const legacy_u16 *line)
{
	memcpy(last_line, line, sizeof(last_line));
	drawn_lines++;
}

static void assert_words(const legacy_s16 *words, legacy_s16 a, legacy_s16 b, legacy_s16 c,
						 legacy_s16 d)
{
	assert(words[0] == a && words[1] == b && words[2] == c && words[3] == d);
}

static void set_line(unsigned index, legacy_s16 x1, legacy_s16 y1, legacy_s16 x2, legacy_s16 y2)
{
	LEGACY_WRITE_U16_LE(crack_lines + index * 8, x1);
	LEGACY_WRITE_U16_LE(crack_lines + index * 8 + 2, y1);
	LEGACY_WRITE_U16_LE(crack_lines + index * 8 + 4, x2);
	LEGACY_WRITE_U16_LE(crack_lines + index * 8 + 6, y2);
}

static void reset_overlay(legacy_s16 dirty_rects)
{
	struct SHAPE3D_LEGACY_OPPONENT_RENDER_CONTEXT context;
	unsigned i;

	memset(&context, 0, sizeof(context));
	context.wheel_headings = opponent;
	shape3d_set_legacy_render_stack(player, 0x9000, 0x4000, &context);
	for (i = 0; i < LEGACY_RESIDUE_WORD_COUNT; i++) {
		player[i] = 100 + i;
		opponent[i] = 200 + i;
	}
	drawing_sprite.sprite_raster_left = 0;
	drawing_sprite.sprite_raster_right = 320;
	drawing_sprite.sprite_top = 0;
	drawing_sprite.sprite_bottom = 200;
	slow_video_mgmt_copy = dirty_rects;
	framespersec = 20;
	dialog_fnt_colour = 7;
	drawn_lines = 0;
	crack_offset = 11;
	LEGACY_WRITE_U16_LE(crack_info, 2);
	LEGACY_WRITE_U16_LE(crack_info + 2, 1);
	LEGACY_WRITE_U16_LE(crack_info + 4, 2);
	set_line(0, 20, 40, 100, 160);
	set_line(1, 140, 40, 200, 100);
}

static void test_incremental_crack_overlay(void)
{
	struct RECTANGLE *bounds;

	reset_overlay(1);
	bounds = init_crak(0, 10, 100);
	assert(drawn_lines == 3);
	assert(last_line[DRAW_LINE_COLOR_INDEX] == 7);
	assert(last_line[DRAW_LINE_START_Y_INDEX] == 30);
	assert(last_line[DRAW_LINE_END_Y_INDEX] == 90);
	assert(bounds->left == 20 && bounds->right == 101);
	assert(bounds->top == 29 && bounds->bottom == 92);
	/* SEG003 init_crak -> SEG012 preRender_line places the first four line
	 * words over player headings; the later bounds calls save SI/DI over the
	 * last two opponent words. These replace the preceding scene's residue. */
	assert_words(player, 0, 20, 0, 30);
	assert_words(opponent, 0, 0, 0, 11);

	/* A later animation frame retains the last line's index and coordinates.
	 * Clamp to the final frame and vary the normalized resource offset. */
	crack_offset = 3;
	init_crak(300, 10, 100);
	assert(drawn_lines == 9);
	assert_words(player, 0, 140, 0, 30);
	assert_words(opponent, 0, 0, 1, 3);
}

static void test_rejected_crack_lines(void)
{
	reset_overlay(1);
	set_line(0, 20, -10, 100, -10);
	init_crak(0, 0, 200);
	assert(drawn_lines == 0);
	assert_words(player, 0, 20, 0, 0);
	assert_words(opponent, 0, 0, 0, 11);

	reset_overlay(1);
	set_line(0, -20, 10, -10, 20);
	init_crak(0, 0, 200);
	assert(drawn_lines == 0);
	assert(opponent[0] == 0 && opponent[1] == 11);
	assert(opponent[2] == 0 && opponent[3] == 11);
}

static void test_direct_redraw_stack(void)
{
	reset_overlay(0);
	init_crak(0, 10, 100);
	assert(drawn_lines == 3);
	/* Without pending rect_union arguments, the line buffer is four bytes
	 * higher. A drawn line leaves SEG012's return CS and the buffer address. */
	assert_words(player, 0x49cc, (legacy_s16)0x8fb0, 0, 20);
	assert_words(opponent, 7, DRAW_LINE_MODE_X_MAJOR_RIGHT, 0, 0);

	reset_overlay(0);
	set_line(0, 20, -10, 100, -10);
	init_crak(0, 0, 200);
	assert(drawn_lines == 0);
	/* A rejected line never makes the pixel call: its setup argument stays. */
	assert_words(player, -10, (legacy_s16)0x8fb0, 0, 20);
	assert_words(opponent, 7,
				 DRAW_LINE_MODE_HORIZONTAL | (DRAW_LINE_CLIP_TOP << DRAW_LINE_CLIP_SHIFT), 0, 0);
}

static void test_explicit_crack_context(void)
{
	reset_overlay(1);
	preRender_line(20, 30, 100, 90, 7);
	assert_words(player, 100, 101, 102, 103);
	assert_words(opponent, 200, 201, 202, 203);

	shape3d_set_legacy_render_stack(0, 0, 0, 0);
	init_crak(0, 10, 100);
	assert(drawn_lines == 4);
	assert_words(player, 100, 101, 102, 103);
	assert_words(opponent, 200, 201, 202, 203);
}

static void reset_ingame_text(const char *filename)
{
	reset_overlay(1);
	video_x_alignment = 1;
	memset(&state, 0, sizeof(state));
	assert(strlen(filename) < sizeof(replay_filename));
	memcpy(replay_filename, filename, strlen(filename) + 1);
	game_replay_mode = REPLAY_MODE_PLAYBACK;
	idle_expired = 0;
	passed_security = 1;
	cameramode = CAMERA_MODE_TRACKSIDE;
	text_draw_count = 0;
	fps_display_enabled = 0;
	frame_fps_reset();
	realtime_ticks = 0;
	roofbmpheight_copy = 0;
	dashboard_visible = 0;
	font_glyph_height = 8;
	video_uses_page_flipping = 0;
	restored_roof_count = copied_roof_count = 0;
	frame_sprite.sprite_bitmapptr = &frame_shape;
	render_window_sprite = &frame_sprite;
}

static void assert_text(unsigned index, const char *text, legacy_s16 x, legacy_s16 y)
{
	assert(index < text_draw_count);
	assert(strcmp(text_draws[index].text, text) == 0);
	assert(text_draws[index].x == x && text_draws[index].y == y);
	assert(text_draws[index].color == dialog_fnt_colour);
	assert(text_draws[index].shadow_color == 0);
}

static void test_replay_filename_survives_blink(void)
{
	static const char *filenames[] = {"A", "DEFAULT", "RACE2026", "MOUNTAINRUN2026",
									  "ABCDEFGHIJKLMNOPQRSTUVWXYZ12345"};
	for (unsigned name = 0; name < sizeof(filenames) / sizeof(filenames[0]); name++) {
		for (unsigned frame_rate = 10; frame_rate <= 20; frame_rate += 10) {
			reset_ingame_text(filenames[name]);
			dialog_fnt_colour = 9 + name;
			framespersec = frame_rate;
			legacy_s16 filename_x = 312 - strlen(filenames[name]) * 8;
			for (unsigned frame = 0; frame < frame_rate * 2; frame++) {
				state.game_frame = frame;
				text_draw_count = 0;
				struct RECTANGLE *bounds = draw_ingame_text();
				assert_text(0, filenames[name], filename_x, 3);
				assert(bounds->top == 3 && bounds->right == 313);
				if (frame % frame_rate < frame_rate / 2) {
					assert(text_draw_count == 2);
					assert_text(1, "Replay", 264, 15);
					assert(bounds->left == (filename_x < 264 ? filename_x : 264));
					assert(bounds->bottom == 24);
				} else {
					assert(text_draw_count == 1);
					/* Incremental redraw must still copy the nonblinking filename. */
					assert(bounds->left == filename_x && bounds->bottom == 12);
				}
			}
		}
	}
}

static void test_wrapped_replay_filename(void)
{
	static const struct {
		unsigned length;
		unsigned lines;
	} cases[] = {{38, 1}, {39, 2}, {76, 2}, {127, 4}};
	for (unsigned name = 0; name < sizeof(cases) / sizeof(cases[0]); name++) {
		char filename[REPLAY_FILENAME_SIZE];
		for (unsigned character = 0; character < cases[name].length; character++) {
			filename[character] = 'A' + character % 26;
		}
		filename[cases[name].length] = 0;
		for (unsigned phase = 0; phase < 3; phase++) {
			reset_ingame_text(filename);
			if (phase == 1) {
				state.game_frame = framespersec / 2;
			} else if (phase == 2) {
				game_replay_mode = REPLAY_MODE_PAUSED;
			}
			struct RECTANGLE *bounds = draw_ingame_text();
			unsigned line_count = cases[name].lines;
			assert(text_draw_count == line_count + (phase == 0));
			char reconstructed[REPLAY_FILENAME_SIZE];
			unsigned copied = 0;
			for (unsigned line = 0; line < line_count; line++) {
				unsigned length = line + 1 == line_count ? cases[name].length - copied : 38;
				char expected[39];
				memcpy(expected, filename + copied, length);
				expected[length] = 0;
				assert_text(line, expected, 312 - length * 8, 3 + line * 12);
				memcpy(reconstructed + copied, text_draws[line].text, length);
				copied += length;
			}
			reconstructed[copied] = 0;
			assert(strcmp(reconstructed, filename) == 0);
			assert(strcmp((const char *)replay_filename, filename) == 0);
			assert(bounds->left == 8 && bounds->right == 313);
			assert(bounds->top == 3);
			if (phase == 0) {
				assert_text(line_count, "Replay", 264, 3 + line_count * 12);
				assert(bounds->bottom == (legacy_s16)((line_count + 1) * 12));
			} else {
				assert(bounds->bottom == (legacy_s16)(line_count * 12));
			}
		}
	}
}

static void test_replay_filename_when_paused(void)
{
	reset_ingame_text("RACE2026");
	game_replay_mode = REPLAY_MODE_PAUSED;
	struct RECTANGLE *bounds = draw_ingame_text();
	assert(text_draw_count == 1);
	assert_text(0, "RACE2026", 248, 3);
	assert(bounds->left == 248 && bounds->right == 313);
	assert(bounds->top == 3 && bounds->bottom == 12);
}

static void test_unnamed_replay_overlay(void)
{
	reset_ingame_text("");
	struct RECTANGLE *bounds = draw_ingame_text();
	assert(text_draw_count == 1);
	assert_text(0, "Replay", 264, 15);
	assert(bounds->top == 15 && bounds->bottom == 24);
	state.game_frame = 10;
	text_draw_count = 0;
	bounds = draw_ingame_text();
	assert(text_draw_count == 0);
	assert(memcmp(bounds, &empty_rect, sizeof(*bounds)) == 0);

	game_replay_mode = REPLAY_MODE_PAUSED;
	bounds = draw_ingame_text();
	assert(text_draw_count == 0);
	assert(memcmp(bounds, &empty_rect, sizeof(*bounds)) == 0);
}

static void test_filename_hidden_in_live_race_and_demo(void)
{
	reset_ingame_text("DEFAULT");
	game_replay_mode = REPLAY_MODE_LIVE;
	state.game_inputmode = GAME_INPUT_MODE_ACTIVE;
	struct RECTANGLE *bounds = draw_ingame_text();
	assert(text_draw_count == 0);
	assert(memcmp(bounds, &empty_rect, sizeof(*bounds)) == 0);

	game_replay_mode = REPLAY_MODE_PLAYBACK;
	idle_expired = 1;
	bounds = draw_ingame_text();
	assert(text_draw_count == 2);
	assert_text(0, "Demo", 144, 170);
	assert_text(1, "Press a key", 116, 182);
	assert(bounds->top == 170 && bounds->bottom == 191);
}

static void assert_fps(const char *expected, legacy_s16 color)
{
	text_draw_count = 0;
	struct RECTANGLE *bounds = draw_ingame_text();
	assert(strcmp(text_draws[0].text, expected) == 0);
	assert(text_draws[0].x == 8 && text_draws[0].y == 3);
	assert(text_draws[0].color == color && text_draws[0].shadow_color == 0);
	assert(bounds->left == 8 && bounds->top == 3);
	assert(bounds->right >= (legacy_s16)(8 + strlen(expected) * 8 + 1));
}

static void present_frames(unsigned frames, legacy_u32 ticks)
{
	legacy_u32 start = realtime_ticks;
	for (unsigned frame = 1; frame <= frames; frame++) {
		realtime_ticks = LEGACY_U32_WRAP_ADD(start, ticks * frame / frames);
		frame_fps_record_presented();
	}
}

static void test_fps_sampling(void)
{
	reset_ingame_text("");
	fps_display_enabled = 1;
	frame_fps_record_presented();
	assert_fps("0 FPS", 4);
	present_frames(20, 100);
	assert_fps("20 FPS", 2);
	/* More than twenty and fractional rates retain floor semantics. */
	present_frames(24, 100);
	assert_fps("24 FPS", 2);
	present_frames(19, 100);
	assert_fps("19 FPS", 4);
	present_frames(20, 101);
	assert_fps("19 FPS", 4);
	/* Rendering without presenting must not change the sample. */
	for (unsigned frame = 0; frame < 50; frame++) {
		assert_fps("19 FPS", 4);
	}
	present_frames(1, 300);
	assert_fps("0 FPS", 4);
	present_frames(20, 100);
	assert_fps("20 FPS", 2);
	fps_display_enabled = 0;
	text_draw_count = 0;
	game_replay_mode = REPLAY_MODE_PAUSED;
	struct RECTANGLE *bounds = draw_ingame_text();
	assert(text_draw_count == 0);
	assert(memcmp(bounds, &empty_rect, sizeof(*bounds)) == 0);
	frame_fps_record_presented();
	fps_display_enabled = 1;
	frame_fps_record_presented();
	assert_fps("0 FPS", 4);
	present_frames(20, 100);
	assert_fps("20 FPS", 2);
	/* Unsigned time differences continue across a 32-bit tick wrap. */
	frame_fps_reset();
	realtime_ticks = LEGACY_U32_MAX - 49UL;
	frame_fps_record_presented();
	present_frames(20, 100);
	assert_fps("20 FPS", 2);
}

static void test_fps_outside_race(void)
{
	reset_ingame_text("REPLAY");
	/* Intro rendering must ignore cockpit and replay state left by a previous race. */
	dashboard_visible = 1;
	roofbmpheight_copy = 20;
	rect_ingame_text = (struct RECTANGLE){100, 200, 150, 190};
	struct RECTANGLE saved_text_rect = rect_ingame_text;
	struct RECTANGLE *bounds = frame_fps_draw_text();
	assert(memcmp(bounds, &empty_rect, sizeof(*bounds)) == 0);
	assert(text_draw_count == 0);
	fps_display_enabled = 1;
	frame_fps_record_presented();
	present_frames(20, DOS_TIMER_REALTIME_TICKS_PER_SECOND);
	bounds = frame_fps_draw_text();
	assert(text_draw_count == 1);
	assert(strcmp(text_draws[0].text, "20 FPS") == 0);
	assert(text_draws[0].x == 8 && text_draws[0].y == 3);
	assert(text_draws[0].color == 2 && text_draws[0].shadow_color == 0);
	assert(bounds->left == 8 && bounds->right == 57);
	assert(bounds->top == 3 && bounds->bottom == 12);
	assert(restored_roof_count == 0 && copied_roof_count == 0);
	assert(memcmp(&rect_ingame_text, &saved_text_rect, sizeof(rect_ingame_text)) == 0);
}

static void test_fps_idle_expiry(void)
{
	reset_ingame_text("");
	fps_display_enabled = 1;
	frame_fps_record_presented();
	present_frames(20, 100);
	assert_fps("20 FPS", 2);
	/* Expiry is measured from the latest presentation, not the sample window. */
	present_frames(10, 50);
	realtime_ticks += 99;
	assert(frame_fps_expire_idle() == 0);
	assert_fps("20 FPS", 2);
	realtime_ticks++;
	assert(frame_fps_expire_idle() == 1);
	assert_fps("0 FPS", 4);
	assert(frame_fps_expire_idle() == 0);
	frame_fps_record_presented();
	realtime_ticks += 200;
	assert(frame_fps_expire_idle() == 0);
	assert_fps("0 FPS", 4);
	/* Resuming rendering starts a fresh sample and can expire again. */
	frame_fps_reset();
	realtime_ticks = LEGACY_U32_MAX - 149UL;
	frame_fps_record_presented();
	present_frames(20, 100);
	assert_fps("20 FPS", 2);
	realtime_ticks = LEGACY_U32_WRAP_ADD(realtime_ticks, 100);
	fps_display_enabled = 0;
	assert(frame_fps_expire_idle() == 0);
	fps_display_enabled = 1;
	assert(frame_fps_expire_idle() == 1);
	assert_fps("0 FPS", 4);
}

static void test_fps_camera_modes(void)
{
	for (legacy_u8 mode = REPLAY_MODE_LIVE; mode <= REPLAY_MODE_PAUSED; mode++) {
		for (legacy_u16 camera = CAMERA_MODE_COCKPIT; camera < CAMERA_MODE_COUNT; camera++) {
			reset_ingame_text("TEST");
			fps_display_enabled = 1;
			game_replay_mode = mode;
			cameramode = camera;
			state.game_inputmode = GAME_INPUT_MODE_ACTIVE;
			followOpponentFlag = 1;
			assert_fps("0 FPS", 4);
		}
	}
}

static void test_fps_and_long_replay_filename(void)
{
	char filename[REPLAY_FILENAME_SIZE];
	memset(filename, 'R', sizeof(filename) - 1);
	filename[sizeof(filename) - 1] = 0;
	reset_ingame_text(filename);
	fps_display_enabled = 1;
	frame_fps_record_presented();
	present_frames(20, 100);
	assert_fps("20 FPS", 2);
	assert(text_draw_count == 6);
	assert(text_draws[1].y == 3);
	assert(text_draws[1].x > text_draws[0].x + (legacy_s16)strlen(text_draws[0].text) * 8);
	char reconstructed[REPLAY_FILENAME_SIZE];
	unsigned copied = 0;
	for (unsigned line = 1; line < text_draw_count - 1; line++) {
		unsigned length = strlen(text_draws[line].text);
		memcpy(reconstructed + copied, text_draws[line].text, length);
		copied += length;
	}
	reconstructed[copied] = 0;
	assert(strcmp(reconstructed, filename) == 0);
	assert_text(text_draw_count - 1, "Replay", 264, 51);
}

static void test_fps_on_cockpit_roof(void)
{
	for (legacy_s16 roof_height = 0; roof_height <= 20; roof_height++) {
		for (legacy_s16 page_flipping = 0; page_flipping <= 1; page_flipping++) {
			reset_ingame_text("");
			fps_display_enabled = 1;
			dashboard_visible = 1;
			roofbmpheight_copy = roof_height;
			video_uses_page_flipping = page_flipping;
			sprite_set_target_clip_bounds(0, 320, roof_height, 200);
			struct SPRITE original_context[SPRITE_STATE_COUNT];
			sprite_save_context(original_context);
			assert_fps("0 FPS", 4);
			assert(memcmp(&drawing_sprite, original_context, sizeof(drawing_sprite)) == 0);
			assert(memcmp(&screen_sprite, original_context + 1, sizeof(screen_sprite)) == 0);
			frame_fps_present_roof();
			assert(memcmp(&drawing_sprite, original_context, sizeof(drawing_sprite)) == 0);
			assert(memcmp(&screen_sprite, original_context + 1, sizeof(screen_sprite)) == 0);
			assert(restored_roof_count == (unsigned)(roof_height > 3));
			assert(copied_roof_count == (unsigned)(roof_height > 3 && page_flipping == 0));
			if (roof_height > 3) {
				assert(restored_roof_bounds.left == 8 && restored_roof_bounds.right == 81);
				assert(restored_roof_bounds.top == 3);
				assert(restored_roof_bounds.bottom == (roof_height < 12 ? roof_height : 12));
				if (page_flipping == 0) {
					assert(memcmp(&copied_roof_bounds, &restored_roof_bounds,
								  sizeof(copied_roof_bounds)) == 0);
				}
			}
			/* Hidden dashboards and disabled FPS never touch the roof. */
			dashboard_visible = 0;
			assert_fps("0 FPS", 4);
			frame_fps_present_roof();
			assert(restored_roof_count == (unsigned)(roof_height > 3));
			assert(copied_roof_count == (unsigned)(roof_height > 3 && page_flipping == 0));
		}
	}
}

int main(void)
{
	test_incremental_crack_overlay();
	test_rejected_crack_lines();
	test_direct_redraw_stack();
	test_explicit_crack_context();
	test_replay_filename_survives_blink();
	test_wrapped_replay_filename();
	test_replay_filename_when_paused();
	test_unnamed_replay_overlay();
	test_filename_hidden_in_live_race_and_demo();
	test_fps_sampling();
	test_fps_outside_race();
	test_fps_idle_expiry();
	test_fps_camera_modes();
	test_fps_and_long_replay_filename();
	test_fps_on_cockpit_roof();
	return 0;
}
