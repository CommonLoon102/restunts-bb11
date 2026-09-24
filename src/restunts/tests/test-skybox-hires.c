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
#include "../c/skybox.h"

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

#define PANORAMA_IMAGE_COUNT 4

legacy_u16 projection_center_x = 160;
legacy_u16 projection_center_y = 100;
legacy_u16 projection_focal_length_x = 160;
legacy_u16 projection_focal_length_y = 160;

static const legacy_s32 panorama_widths[PANORAMA_IMAGE_COUNT] = {320, 192, 320, 192};
static const legacy_s32 panorama_heights[PANORAMA_IMAGE_COUNT] = {24, 32, 40, 48};
static const char *panorama_paths[PANORAMA_IMAGE_COUNT] = {
	"skyboxes/city-scen.png", "skyboxes/city-sce2.png", "skyboxes/city-sce3.png",
	"skyboxes/city-sce4.png"};
static struct SHAPE2D *panorama_shapes[PANORAMA_IMAGE_COUNT];
static struct SKYBOX scenery = {{24, 32, 40, 48}, 24, 48, 1, 2, 4};

static legacy_u8 panorama_color(legacy_s32 image, legacy_s32 x, legacy_s32 y)
{
	return 17 + (image * 13 + x * 3 + y * 11) % 63;
}

static legacy_u8 original_color(legacy_s32 image, legacy_s32 x, legacy_s32 y)
{
	return 96 + image * 12 + x % 3 + y % 4 * 3;
}

static void create_panorama_fixtures(void)
{
	for (legacy_s32 image = 0; image < PANORAMA_IMAGE_COUNT; image++) {
		legacy_s32 width = panorama_widths[image];
		legacy_s32 height = panorama_heights[image];
		struct SHAPE2D *shape = calloc(1, sizeof(*shape) + width * height);
		assert(shape != NULL);
		shape->width = width;
		shape->height = height;
		panorama_shapes[image] = shape;
		legacy_u8 *original = (legacy_u8 *)(shape + 1);
		for (legacy_s32 y = 0; y < height; y++) {
			for (legacy_s32 x = 0; x < width; x++) {
				original[y * width + x] = original_color(image, x, y);
			}
		}
		SDL_Surface *source = SDL_CreateSurface(width * 4, height * 4, SDL_PIXELFORMAT_RGB24);
		assert(source != NULL);
		for (legacy_s32 y = 0; y < height * 4; y++) {
			for (legacy_s32 x = 0; x < width * 4; x++) {
				legacy_u8 shade = (panorama_color(image, x, y) - 16U) * 255U / 63U;
				assert(SDL_WriteSurfacePixel(source, x, y, shade, shade, shade, 255));
			}
		}
		assert(SDL_SavePNG(source, panorama_paths[image]));
		SDL_DestroySurface(source);
	}
}

static void destroy_panorama_fixtures(void)
{
	for (legacy_s32 image = 0; image < PANORAMA_IMAGE_COUNT; image++) {
		assert(remove(panorama_paths[image]) == 0);
		free(panorama_shapes[image]);
	}
}

static struct MATRIX roll_matrix(legacy_s32 angle)
{
	legacy_f64 radians = angle * (2.0 * SDL_PI_D / 1024.0);
	legacy_s16 cosine = SDL_lround(SDL_cos(radians) * TRIG_FIXED_ONE);
	legacy_s16 sine = SDL_lround(SDL_sin(radians) * TRIG_FIXED_ONE);
	struct MATRIX rotation = {0};
	rotation.m._11 = cosine;
	rotation.m._21 = sine;
	rotation.m._12 = -sine;
	rotation.m._22 = cosine;
	rotation.m._33 = TRIG_FIXED_ONE;
	return rotation;
}

/* Cardinal rotations give exact source texels, independently of the renderer's
 * general rotation/projection calculation. band_y is the source row relative
 * to the panorama's bottom edge, in enhanced pixels. */
static legacy_u8 expected_panorama(legacy_s32 u, legacy_s32 band_y, legacy_s32 original_mask,
								   legacy_s32 detail)
{
	if (band_y >= 0) {
		return scenery.ground_color;
	}
	if (detail == 4) {
		return scenery.sky_color;
	}
	u = (u % 4096 + 4096) % 4096;
	legacy_s32 image = 0;
	while (u >= panorama_widths[image] * 4) {
		u -= panorama_widths[image] * 4;
		image++;
	}
	legacy_s32 row = panorama_heights[image] * 4 + band_y;
	if (row < 0) {
		return scenery.sky_color;
	}
	if (original_mask & (1 << image)) {
		return original_color(image, u / 4, row / 4);
	}
	return panorama_color(image, u, row);
}

static void assert_legacy_unchanged(void)
{
	for (legacy_s32 index = 0; index < 320 * 200; index++) {
		assert(screen[index] == 3);
	}
}

static void assert_cardinal_image(legacy_s32 quarter_turns, legacy_s32 heading, legacy_s32 altitude,
								  legacy_s32 original_mask, legacy_s32 detail)
{
	const legacy_u8 *output = pixels();
	for (legacy_s32 y = 0; y < HIRES_HEIGHT; y++) {
		for (legacy_s32 x = 0; x < HIRES_WIDTH; x++) {
			legacy_s32 u;
			legacy_s32 band_y;
			switch (quarter_turns) {
				case 0:
					u = x;
					band_y = y - 400;
					break;
				case 1:
					u = 1039 - y;
					band_y = x - 640;
					break;
				case 2:
					u = 1279 - x;
					band_y = 399 - y;
					break;
				default:
					u = 240 + y;
					band_y = 639 - x;
					break;
			}
			legacy_u8 expected = 3;
			if (x >= target.sprite_raster_left * 4 && x < target.sprite_raster_right * 4 &&
				y >= target.sprite_top * 4 && y < target.sprite_bottom * 4) {
				expected = expected_panorama(u + heading, band_y + altitude, original_mask, detail);
			}
			assert(output[y * HIRES_WIDTH + x] == expected);
		}
	}
	assert_legacy_unchanged();
}

static void test_cardinal_rotations_and_wrap(void)
{
	for (legacy_s32 quarter = 0; quarter <= 4; quarter++) {
		struct MATRIX rotation = roll_matrix(quarter * 256);
		reset_target();
		assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 0));
		assert_cardinal_image(quarter % 4, 0, 0, 0, 0);
	}
	/* These headings put each of the differently sized strips at the left
	 * edge and cross their joins; full-turn headings wrap without a seam. */
	static const legacy_s16 headings[] = {192, 0, 704, 1536, -512};
	struct MATRIX rotation = roll_matrix(0);
	for (legacy_s32 index = 0; index < (legacy_s32)SDL_arraysize(headings); index++) {
		reset_target();
		assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1,
								   headings[index], 0, 0));
		assert_cardinal_image(0, -((headings[index] + 512) & 1023) * 4, 0, 0, 0);
	}
}

static void test_banked_roads_and_corkscrew(void)
{
	/* The old horizon-endpoint cutoff lies between the 44/1024 and 52/1024
	 * rolls with this viewport. Check both banks and a complete corkscrew. */
	static const legacy_s16 rolls[] = {40,	44,	 48,  52,  64,	-40, -44, -48, -52, -64, 128, 192,
									   256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960};
	for (legacy_s32 index = 0; index < (legacy_s32)SDL_arraysize(rolls); index++) {
		reset_target();
		struct MATRIX rotation = roll_matrix(rolls[index]);
		assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 0));
		const legacy_u8 *output = pixels();
		legacy_s32 scenery_pixels = 0;
		for (legacy_s32 pixel = 0; pixel < HIRES_WIDTH * HIRES_HEIGHT; pixel++) {
			scenery_pixels += output[pixel] >= 17 && output[pixel] <= 79;
		}
		assert(scenery_pixels > 16000);
		assert_legacy_unchanged();
	}
}

static void test_pitch_poles_and_inverted_altitude(void)
{
	for (legacy_s16 direction = -1; direction <= 1; direction += 2) {
		reset_target();
		struct MATRIX rotation = {0};
		rotation.m._11 = TRIG_FIXED_ONE;
		rotation.m._32 = direction * TRIG_FIXED_ONE;
		rotation.m._23 = -direction * TRIG_FIXED_ONE;
		assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 0));
		const legacy_u8 *output = pixels();
		legacy_u8 expected = direction > 0 ? scenery.sky_color : scenery.ground_color;
		for (legacy_s32 pixel = 0; pixel < HIRES_WIDTH * HIRES_HEIGHT; pixel++) {
			assert(output[pixel] == expected);
		}
		assert_legacy_unchanged();
	}
	/* A half-turn in pitch reverses both world up and the altitude correction.
	 * At height 1500, the horizon moves down 16 logical pixels. Rear views
	 * reverse the altitude correction and advance the panorama by half a turn. */
	struct MATRIX rotation = {0};
	rotation.m._11 = TRIG_FIXED_ONE;
	rotation.m._22 = -TRIG_FIXED_ONE;
	rotation.m._33 = -TRIG_FIXED_ONE;
	for (legacy_s16 direction = -1; direction <= 1; direction += 2) {
		reset_target();
		assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, direction, 512,
								   1500, 0));
		assert_cardinal_image(2, direction < 0 ? 2048 : 0, direction * 64, 0, 0);
	}
}

static void test_anisotropic_projection(void)
{
	reset_target();
	projection_focal_length_x = 200;
	projection_focal_length_y = 100;
	struct MATRIX rotation = roll_matrix(256);
	assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 0));
	const legacy_u8 *output = pixels();
	/* With focal lengths in a 2:1 ratio, a quarter-turn makes the panorama
	 * twice as wide vertically and halves its height horizontally. */
	for (legacy_s32 y = 0; y < HIRES_HEIGHT; y++) {
		for (legacy_s32 x = 0; x < HIRES_WIDTH; x++) {
			legacy_u8 expected = expected_panorama(1439 - 2 * y, x / 2 - 320, 0, 0);
			assert(output[y * HIRES_WIDTH + x] == expected);
		}
	}
	assert_legacy_unchanged();
	projection_focal_length_x = 160;
	projection_focal_length_y = 160;
}

static void test_oriented_clipping_fallback_and_toggle(void)
{
	struct MATRIX rotation = roll_matrix(768);
	reset_target();
	target.sprite_raster_left = 17;
	target.sprite_raster_right = 299;
	target.sprite_top = 11;
	target.sprite_bottom = 173;
	assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 0));
	assert_cardinal_image(3, 0, 0, 0, 0);
	/* The desert fixtures are absent, malformed, or the wrong dimensions.
	 * Every strip must still rotate correctly using its original pixels. */
	for (legacy_s32 quarter = 0; quarter < 4; quarter++) {
		reset_target();
		rotation = roll_matrix(quarter * 256);
		assert(skybox_hires_render(&target, &scenery, panorama_shapes, 0, &rotation, 1, 0, 0, 0));
		assert_cardinal_image(quarter, -2048, 0, 15, 0);
	}
	reset_target();
	assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 4));
	assert_cardinal_image(3, 0, 0, 0, 4);
	reset_target();
	hires_set_enabled(0);
	assert(!skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 0));
	hires_set_enabled(1);
	assert_plain();
	projection_focal_length_x = 0;
	assert(!skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 0));
	projection_focal_length_x = 160;
	projection_focal_length_y = 0;
	assert(!skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 0));
	projection_focal_length_y = 160;
	assert_plain();
	assert_legacy_unchanged();
	assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 0));
	assert_cardinal_image(3, 0, 0, 0, 0);
}

static void test_mixed_artwork_and_view_transitions(void)
{
	reset_target();
	const char *hidden_path = "skyboxes/city-sce2.hidden.png";
	assert(rename(panorama_paths[1], hidden_path) == 0);
	struct MATRIX rotation = roll_matrix(0);
	assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 192, 0, 0));
	/* The second strip uses original pixels next to the enhanced third strip. */
	assert_cardinal_image(0, -704 * 4, 0, 1 << 1, 0);
	assert(rename(hidden_path, panorama_paths[1]) == 0);
	skybox_hires_unload();
	/* Consecutive views must overwrite every prior sky/ground/scenery pixel,
	 * including when the horizon changes from vertical to inverted. */
	rotation = roll_matrix(256);
	assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 0));
	assert_cardinal_image(1, 0, 0, 0, 0);
	rotation = roll_matrix(512);
	assert(skybox_hires_render(&target, &scenery, panorama_shapes, 3, &rotation, 1, 512, 0, 0));
	assert_cardinal_image(2, 0, 0, 0, 0);
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
	create_panorama_fixtures();
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
	test_cardinal_rotations_and_wrap();
	test_banked_roads_and_corkscrew();
	test_pitch_poles_and_inverted_altitude();
	test_anisotropic_projection();
	test_oriented_clipping_fallback_and_toggle();
	test_mixed_artwork_and_view_transitions();
	test_copies_overlays_and_theme_changes();
	skybox_hires_unload();
	hires_shutdown();
	destroy_panorama_fixtures();
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
