#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#include "../c/externs.h"
#include "../c/frame_internal.h"
#include "../c/hires.h"
#include "../c/menu_internal.h"
#include "../c/platform.h"
#include "../c/projection.h"
#include "../c/shape2d.h"
#include "../c/skybox.h"
#include "../c/skybox_hires.h"

#define SKY_HEIGHT 8
#define HORIZON 80
#define LEGACY_SKY_COLOR 4

static struct SPRITE preview, screen_target;
static legacy_u8 preview_rows[200 * 2], screen_rows[200 * 2];
static legacy_u8 empty_track[900];
static legacy_u8 *screen;

static legacy_u8 fixture_color(legacy_s32 x, legacy_s32 y)
{
	return 17 + (x + y * HIRES_WIDTH) % 63;
}

static void write_fixture(void)
{
	SDL_Surface *image =
		SDL_CreateSurface(HIRES_WIDTH, SKY_HEIGHT * HIRES_SCALE, SDL_PIXELFORMAT_RGB24);
	assert(image != NULL);
	for (legacy_s32 y = 0; y < image->h; y++) {
		for (legacy_s32 x = 0; x < image->w; x++) {
			legacy_u8 shade = (fixture_color(x, y) - 16U) * 255U / 63U;
			assert(SDL_WriteSurfacePixel(image, x, y, shade, shade, shade, 255));
		}
	}
	assert(SDL_SavePNG(image, "skyboxes/country-sce3.png"));
	SDL_DestroySurface(image);
}

static void prepare_sprite(struct SPRITE *sprite, legacy_u8 *rows, legacy_u16 segment,
						   legacy_u16 header_size)
{
	sprite->sprite_bitmapptr = dos_memory_make_pointer(segment, 0);
	sprite->sprite_lineofs = rows;
	sprite->sprite_right = sprite->sprite_pitch = sprite->sprite_buffer_width = 320;
	sprite->sprite_raster_right = 320;
	sprite->sprite_bottom = 200;
	for (legacy_u32 row = 0; row < 200; row++) {
		LEGACY_WRITE_U16_LE(rows + row * 2, header_size + row * 320);
	}
	if (header_size != 0) {
		memset(sprite->sprite_bitmapptr, 0, header_size);
		sprite->sprite_bitmapptr->width = 320;
		sprite->sprite_bitmapptr->height = 200;
	}
}

static void assert_frame(legacy_s32 enhanced)
{
	legacy_s32 width, height;
	const legacy_u8 *pixels = hires_framebuffer(screen, &width, &height);
	legacy_s32 scale = enhanced ? HIRES_SCALE : 1;
	assert(width == 320 * scale && height == 200 * scale);
	for (legacy_s32 y = 0; y < height; y++) {
		for (legacy_s32 x = 0; x < width; x++) {
			legacy_u8 expected = skybox.sky_color;
			if (y >= HORIZON * scale) {
				expected = skybox.ground_color;
			} else if (y >= (HORIZON - SKY_HEIGHT) * scale) {
				expected = enhanced ? fixture_color(x, y - (HORIZON - SKY_HEIGHT) * scale)
									: LEGACY_SKY_COLOR;
			}
			assert(pixels[y * width + x] == expected);
		}
	}
}

static void test_preview(legacy_s32 enhanced)
{
	hires_set_enabled(enhanced);
	loaded_skybox_index = 4;
	sprite_select_target(&preview);
	sprite_clear_target((legacy_u8)skybox.ground_color);
	draw_track_preview();
	/* The track menu releases the source artwork before presenting its window. */
	unload_skybox();
	sprite_select_target(&screen_target);
	sprite_clear_target(0);
	/* Cover both initial menu dissolve and later immediate window refresh. */
	for (legacy_u16 phase = 0; phase < 4; phase++) {
		sprite_draw_dissolve_phase(preview.sprite_bitmapptr, phase);
	}
	assert_frame(enhanced);
	sprite_clear_target(0);
	sprite_putimage(preview.sprite_bitmapptr);
	assert_frame(enhanced);
	assert(screen[(HORIZON - SKY_HEIGHT) * 320] == LEGACY_SKY_COLOR);
}

legacy_int main(void)
{
	assert(SDL_Init(0));
	assert(SDL_CreateDirectory("fixtures"));
	assert(chdir("fixtures") == 0);
	assert(SDL_CreateDirectory("skyboxes"));
	write_fixture();
	legacy_u8 palette[768] = {0};
	for (legacy_u32 index = 0; index < 64; index++) {
		memset(palette + (16 + index) * 3, index, 3);
	}
	skybox_hires_set_palette(palette);
	prepare_sprite(&preview, preview_rows, 0x5000, SHAPE2D_HEADER_SIZE);
	prepare_sprite(&screen_target, screen_rows, 0xA000, 0);
	screen = (legacy_u8 *)screen_target.sprite_bitmapptr;
	struct SHAPE2D *strip = dos_memory_make_pointer(0x3000, 0);
	memset(strip, 0, SHAPE2D_HEADER_SIZE);
	strip->width = 320;
	strip->height = SKY_HEIGHT;
	memset((legacy_u8 *)strip + SHAPE2D_HEADER_SIZE, LEGACY_SKY_COLOR, 320 * SKY_HEIGHT);
	for (legacy_u32 index = 0; index < 4; index++) {
		skyboxes[index] = strip;
		skybox.heights[index] = SKY_HEIGHT;
	}
	skybox.minimum_height = skybox.maximum_height = SKY_HEIGHT;
	skybox.sky_color = 3;
	skybox.ground_color = 6;
	track_element_map = track_terrain_map = empty_track;
	track_preview_camera_x = track_preview_camera_y = track_preview_camera_z = 0;
	track_preview_target_x = track_preview_target_y = 0;
	track_preview_target_z = 1000;
	track_preview_horizon_vector.x = track_preview_horizon_vector.y = 0;
	track_preview_horizon_vector.z = 1000;
	projection_center_x = projection_focal_length_x = projection_focal_length_y = 160;
	projection_center_y = HORIZON;
	test_preview(0);
	test_preview(1);
	test_preview(0);
	hires_shutdown();
	assert(remove("skyboxes/country-sce3.png") == 0);
	assert(rmdir("skyboxes") == 0);
	assert(chdir("..") == 0);
	assert(rmdir("fixtures") == 0);
	SDL_Quit();
	puts("Track preview skybox and menu presentation tests passed.");
	return 0;
}
