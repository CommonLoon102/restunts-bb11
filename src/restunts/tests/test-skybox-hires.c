#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#include "../c/hires.h"
#include "../c/platform.h"
#include "../c/shape2d.h"
#include "../c/shape2d_internal.h"
#include "../c/skybox_hires.h"

static struct SPRITE target;
static legacy_u8 rows[200 * 2];
static legacy_u8 *screen;

void dos_process_exit(legacy_s16 status)
{
	exit(status);
}

static void reset_target(void)
{
	hires_shutdown();
	skybox_hires_unload();
	memset(screen, 3, 320 * 200);
	target.sprite_raster_left = 0;
	target.sprite_raster_right = 320;
	target.sprite_top = 0;
	target.sprite_bottom = 200;
	hires_set_enabled(1);
}

static const legacy_u8 *pixels(void)
{
	legacy_s32 width;
	legacy_s32 height;
	const legacy_u8 *result = hires_framebuffer(screen, &width, &height);
	assert(width == HIRES_WIDTH && height == HIRES_HEIGHT);
	return result;
}

static legacy_u8 fixture_color(legacy_s32 x, legacy_s32 y)
{
	return 17 + (x + y * 8) % 63;
}

static void write_fixture(const char *path)
{
	SDL_Surface *source = SDL_CreateSurface(8, 8, SDL_PIXELFORMAT_RGB24);
	assert(source != NULL);
	for (legacy_s32 y = 0; y < 8; y++) {
		for (legacy_s32 x = 0; x < 8; x++) {
			legacy_u8 shade = (fixture_color(x, y) - 16U) * 255U / 63U;
			assert(SDL_WriteSurfacePixel(source, x, y, shade, shade, shade, 255));
		}
	}
	assert(SDL_SavePNG(source, path));
	SDL_DestroySurface(source);
}

static void test_detail_clipping_and_legacy(void)
{
	reset_target();
	skybox_hires_draw(&target, 0, 0, 2, 2, -1, -1);
	const legacy_u8 *output = pixels();
	for (legacy_s32 y = 0; y < HIRES_HEIGHT; y++) {
		for (legacy_s32 x = 0; x < HIRES_WIDTH; x++) {
			legacy_u8 expected = x < 4 && y < 4 ? fixture_color(x + 4, y + 4) : 3;
			assert(output[y * HIRES_WIDTH + x] == expected);
		}
	}
	for (legacy_s32 index = 0; index < 320 * 200; index++) {
		assert(screen[index] == 3);
	}
	reset_target();
	target.sprite_raster_left = 1;
	target.sprite_raster_right = 2;
	target.sprite_top = 1;
	target.sprite_bottom = 2;
	skybox_hires_draw(&target, 0, 0, 2, 2, 0, 0);
	output = pixels();
	assert(output[4 * HIRES_WIDTH + 4] == fixture_color(4, 4));
	assert(output[4 * HIRES_WIDTH + 3] == 3);
	assert(output[3 * HIRES_WIDTH + 4] == 3);
	assert(output[4 * HIRES_WIDTH + 8] == 3);
	assert(output[8 * HIRES_WIDTH + 4] == 3);
}

static void assert_plain(void)
{
	const legacy_u8 *output = pixels();
	for (legacy_s32 index = 0; index < HIRES_WIDTH * HIRES_HEIGHT; index++) {
		assert(output[index] == 3);
	}
}

static void test_fallback_and_toggle(void)
{
	reset_target();
	/* Missing, malformed, and incorrectly sized images retain the legacy sky. */
	skybox_hires_draw(&target, 0, 1, 2, 2, 0, 0);
	skybox_hires_draw(&target, 0, 2, 2, 2, 0, 0);
	skybox_hires_draw(&target, 0, 0, 3, 2, 0, 0);
	assert_plain();
	reset_target();
	skybox_hires_draw(&target, -1, 0, 2, 2, 0, 0);
	skybox_hires_draw(&target, 5, 0, 2, 2, 0, 0);
	skybox_hires_draw(&target, 0, 4, 2, 2, 0, 0);
	skybox_hires_draw(&target, 0, 0, 2, 2, -20, -20);
	skybox_hires_draw(&target, 0, 0, 2, 2, 320, 200);
	assert_plain();
	hires_set_enabled(0);
	skybox_hires_draw(&target, 0, 0, 2, 2, 0, 0);
	hires_set_enabled(1);
	assert_plain();
	skybox_hires_draw(&target, 0, 0, 2, 2, 0, 0);
	assert(pixels()[0] == fixture_color(0, 0));
	hires_set_enabled(0);
	legacy_s32 width;
	legacy_s32 height;
	assert(hires_framebuffer(screen, &width, &height) == screen);
	assert(width == 320 && height == 200);
}

static void test_copies_overlays_and_theme_changes(void)
{
	reset_target();
	skybox_hires_draw(&target, 0, 0, 2, 2, 0, 0);
	hires_raster(screen, 10, screen, 0, 1, SHAPE2D_RASTER_COPY, NULL);
	assert(pixels()[40] == fixture_color(0, 0));
	assert(pixels()[43] == fixture_color(3, 0));
	hires_write(screen, 10, 3);
	assert(pixels()[40] == 3);
	skybox_hires_draw(&target, 1, 0, 2, 2, 0, 0);
	/* Changing to a missing theme cannot reuse the previous theme's image. */
	hires_write(screen, 0, 3);
	skybox_hires_draw(&target, 1, 0, 2, 2, 0, 0);
	assert(pixels()[0] == 3);
	skybox_hires_draw(&target, 0, 0, 2, 2, 0, 0);
	assert(pixels()[0] == fixture_color(0, 0));
	hires_write(screen, 0, 3);
	skybox_hires_draw(&target, 2, 0, 2, 2, 0, 0);
	assert(pixels()[0] == fixture_color(0, 0));
	/* Palette changes remap the source without consulting the display palette. */
	legacy_u8 black_palette[768] = {0};
	skybox_hires_set_palette(black_palette);
	skybox_hires_draw(&target, 0, 0, 2, 2, 0, 0);
	assert(pixels()[0] == 0);
}

int main(void)
{
	assert(SDL_Init(0));
	screen = dos_memory_make_pointer(0xA000, 0);
	target.sprite_bitmapptr = (struct SHAPE2D *)screen;
	target.sprite_lineofs = rows;
	target.sprite_pitch = 320;
	for (legacy_s32 y = 0; y < 200; y++) {
		LEGACY_WRITE_U16_LE(rows + y * 2, y * 320);
	}
	/* Keep fixtures away from installed or user-provided skybox images. */
	assert(SDL_CreateDirectory("skytest"));
	assert(chdir("skytest") == 0);
	assert(SDL_CreateDirectory("skyboxes"));
	write_fixture("skyboxes/desert-scen.png");
	write_fixture("skyboxes/sky2-0.png");
	FILE *invalid = fopen("skyboxes/desert-sce3.png", "wb");
	assert(invalid != NULL);
	assert(fputs("not a PNG", invalid) >= 0);
	assert(fclose(invalid) == 0);
	legacy_u8 palette[768] = {0};
	for (legacy_s32 index = 0; index < 64; index++) {
		memset(palette + (16 + index) * 3, index, 3);
	}
	skybox_hires_set_palette(palette);
	test_detail_clipping_and_legacy();
	test_fallback_and_toggle();
	test_copies_overlays_and_theme_changes();
	skybox_hires_unload();
	hires_shutdown();
	assert(remove("skyboxes/desert-scen.png") == 0);
	assert(remove("skyboxes/sky2-0.png") == 0);
	assert(remove("skyboxes/desert-sce3.png") == 0);
	assert(rmdir("skyboxes") == 0);
	assert(chdir("..") == 0);
	assert(rmdir("skytest") == 0);
	SDL_Quit();
	puts("High-resolution skybox loading and composition tests passed.");
	return 0;
}
