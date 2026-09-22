#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../c/hires.h"
#include "../c/platform.h"
#include "../c/shape2d.h"
#include "../c/shape2d_internal.h"

#define TEST_WIDTH 320
#define TEST_HEIGHT 200
#define TEST_SCALE 4
#define TEST_HIRES_WIDTH (TEST_WIDTH * TEST_SCALE)
#define TEST_HIRES_HEIGHT (TEST_HEIGHT * TEST_SCALE)
#define TEST_BYTES (TEST_WIDTH * TEST_HEIGHT)

struct TEST_SURFACE {
	struct SPRITE sprite;
	legacy_u8 lines[TEST_HEIGHT * 2];
	legacy_u8 *base;
	legacy_u16 first_pixel;
};

void dos_process_exit(legacy_s16 status)
{
	exit(status);
}

static void setup_surface(struct TEST_SURFACE *surface, legacy_u16 segment, legacy_u16 first_pixel)
{
	memset(surface, 0, sizeof(*surface));
	surface->base = dos_memory_make_pointer(segment, 0);
	surface->first_pixel = first_pixel;
	memset(surface->base, 3, TEST_BYTES + first_pixel);
	surface->sprite.sprite_bitmapptr = (struct SHAPE2D *)surface->base;
	surface->sprite.sprite_lineofs = surface->lines;
	surface->sprite.sprite_right = TEST_WIDTH;
	surface->sprite.sprite_bottom = TEST_HEIGHT;
	surface->sprite.sprite_pitch = TEST_WIDTH;
	surface->sprite.sprite_buffer_width = TEST_WIDTH;
	surface->sprite.sprite_raster_right = TEST_WIDTH;
	for (legacy_u32 y = 0; y < TEST_HEIGHT; y++) {
		legacy_u16 offset = first_pixel + y * TEST_WIDTH;
		LEGACY_WRITE_U16_LE(surface->lines + y * 2U, offset);
	}
	if (first_pixel != 0) {
		surface->sprite.sprite_bitmapptr->width = TEST_WIDTH;
		surface->sprite.sprite_bitmapptr->height = TEST_HEIGHT;
	}
}

static legacy_u16 pixel_offset(const struct TEST_SURFACE *surface, legacy_u32 x, legacy_u32 y)
{
	return surface->first_pixel + y * TEST_WIDTH + x;
}

static void write_pixel(struct TEST_SURFACE *surface, legacy_u32 x, legacy_u32 y, legacy_u8 color)
{
	legacy_u16 offset = pixel_offset(surface, x, y);
	hires_write(surface->base, offset, color);
	surface->base[offset] = color;
}

static void raster_pixel(struct TEST_SURFACE *destination, legacy_u32 destination_x,
						 legacy_u32 destination_y, struct TEST_SURFACE *source, legacy_u32 source_x,
						 legacy_u32 source_y, legacy_s16 operation, const legacy_u8 *palette)
{
	legacy_u16 destination_offset = pixel_offset(destination, destination_x, destination_y);
	legacy_u16 source_offset = pixel_offset(source, source_x, source_y);
	hires_raster(destination->base, destination_offset, source->base, source_offset, 1, operation,
				 palette);
	legacy_u8 value = source->base[source_offset];
	if (operation == SHAPE2D_RASTER_AND) {
		destination->base[destination_offset] &= value;
	} else if (operation == SHAPE2D_RASTER_OR) {
		destination->base[destination_offset] |= value;
	} else if (operation == SHAPE2D_RASTER_MAP) {
		if (palette[value] != 255U) {
			destination->base[destination_offset] = palette[value];
		}
	} else {
		destination->base[destination_offset] = value;
	}
}

static const legacy_u8 *get_framebuffer(const struct TEST_SURFACE *screen)
{
	legacy_s32 width = 0;
	legacy_s32 height = 0;
	const legacy_u8 *pixels = hires_framebuffer(screen->base, &width, &height);
	assert(pixels != NULL);
	assert(width == TEST_HIRES_WIDTH);
	assert(height == TEST_HIRES_HEIGHT);
	return pixels;
}

static void assert_block(const struct TEST_SURFACE *screen, legacy_u32 x, legacy_u32 y,
						 legacy_u8 color)
{
	const legacy_u8 *pixels = get_framebuffer(screen);
	for (legacy_u32 row = 0; row < TEST_SCALE; row++) {
		for (legacy_u32 column = 0; column < TEST_SCALE; column++) {
			assert(pixels[(y * TEST_SCALE + row) * TEST_HIRES_WIDTH + x * TEST_SCALE + column] ==
				   color);
		}
	}
}

static void draw_detail(struct TEST_SURFACE *surface, legacy_u32 x, legacy_u32 y)
{
	hires_begin(&surface->sprite);
	/* Ordinary legacy rendering writes a fallback pixel while the new renderer
	 * retains sixteen individually rasterized samples for presentation. */
	write_pixel(surface, x, y, 3);
	for (legacy_u32 row = 0; row < TEST_SCALE; row++) {
		for (legacy_u32 column = 0; column < TEST_SCALE; column++) {
			hires_pixel(x * TEST_SCALE + column, y * TEST_SCALE + row,
						(legacy_u8)(16U + row * TEST_SCALE + column));
		}
	}
	hires_end();
}

static void assert_detail(const struct TEST_SURFACE *screen, legacy_u32 x, legacy_u32 y,
						  legacy_u8 mask, legacy_u8 addition)
{
	const legacy_u8 *pixels = get_framebuffer(screen);
	for (legacy_u32 row = 0; row < TEST_SCALE; row++) {
		for (legacy_u32 column = 0; column < TEST_SCALE; column++) {
			legacy_u8 expected = ((16U + row * TEST_SCALE + column) & mask) | addition;
			assert(pixels[(y * TEST_SCALE + row) * TEST_HIRES_WIDTH + x * TEST_SCALE + column] ==
				   expected);
		}
	}
}

static void test_detail_and_overlays(struct TEST_SURFACE *screen, struct TEST_SURFACE *window)
{
	draw_detail(window, 10, 20);
	raster_pixel(screen, 30, 40, window, 10, 20, SHAPE2D_RASTER_COPY, NULL);
	assert_detail(screen, 30, 40, 255, 0);
	assert_block(screen, 29, 40, 3);
	/* The UI may write exactly the same palette index as the low-resolution
	 * fallback. This still must replace every detailed sample in that block. */
	write_pixel(screen, 30, 40, 3);
	assert_block(screen, 30, 40, 3);
	raster_pixel(screen, 30, 40, window, 10, 20, SHAPE2D_RASTER_COPY, NULL);
	write_pixel(window, 11, 20, 15);
	raster_pixel(screen, 30, 40, window, 11, 20, SHAPE2D_RASTER_AND, NULL);
	assert_detail(screen, 30, 40, 15, 0);
	write_pixel(window, 11, 20, 128);
	raster_pixel(screen, 30, 40, window, 11, 20, SHAPE2D_RASTER_OR, NULL);
	assert_detail(screen, 30, 40, 15, 128);

	legacy_u8 palette[256];
	for (legacy_u32 index = 0; index < sizeof(palette); index++) {
		palette[index] = (legacy_u8)index;
	}
	palette[128] = 255;
	raster_pixel(screen, 30, 40, window, 11, 20, SHAPE2D_RASTER_MAP, palette);
	assert_detail(screen, 30, 40, 15, 128);
	palette[128] = 42;
	raster_pixel(screen, 30, 40, window, 11, 20, SHAPE2D_RASTER_MAP, palette);
	assert_block(screen, 30, 40, 42);
}

static void test_saved_background(struct TEST_SURFACE *screen, struct TEST_SURFACE *window)
{
	draw_detail(screen, 30, 40);
	raster_pixel(window, 15, 25, screen, 30, 40, SHAPE2D_RASTER_COPY, NULL);
	write_pixel(screen, 30, 40, 3);
	assert_block(screen, 30, 40, 3);
	raster_pixel(screen, 30, 40, window, 15, 25, SHAPE2D_RASTER_COPY, NULL);
	assert_detail(screen, 30, 40, 255, 0);

	/* Transparent palette mappings apply to each detailed sample, including
	 * copies whose ordinary 320x200 fallback uses a different palette index. */
	legacy_u8 palette[256];
	for (legacy_u32 index = 0; index < sizeof(palette); index++) {
		palette[index] = index % 2U == 0 ? 255U : (legacy_u8)index;
	}
	write_pixel(screen, 30, 40, 7);
	raster_pixel(screen, 30, 40, window, 15, 25, SHAPE2D_RASTER_MAP, palette);
	const legacy_u8 *pixels = get_framebuffer(screen);
	for (legacy_u32 row = 0; row < TEST_SCALE; row++) {
		for (legacy_u32 column = 0; column < TEST_SCALE; column++) {
			legacy_u8 expected = column % 2U == 0 ? 7U : 16U + row * TEST_SCALE + column;
			assert(
				pixels[(40U * TEST_SCALE + row) * TEST_HIRES_WIDTH + 30U * TEST_SCALE + column] ==
				expected);
		}
	}
}

static void test_clipping(struct TEST_SURFACE *screen)
{
	screen->sprite.sprite_left = 50;
	screen->sprite.sprite_raster_left = 50;
	screen->sprite.sprite_right = 51;
	screen->sprite.sprite_raster_right = 51;
	screen->sprite.sprite_top = 60;
	screen->sprite.sprite_bottom = 61;
	hires_begin(&screen->sprite);
	hires_pixel(199, 240, 77);
	hires_pixel(200, 239, 77);
	hires_pixel(204, 240, 77);
	hires_pixel(200, 244, 77);
	hires_pixel(-1, -1, 77);
	hires_pixel(TEST_HIRES_WIDTH, TEST_HIRES_HEIGHT, 77);
	hires_pixel(200, 240, 77);
	hires_pixel(203, 243, 78);
	hires_end();
	const legacy_u8 *pixels = get_framebuffer(screen);
	assert(pixels[240U * TEST_HIRES_WIDTH + 200U] == 77);
	assert(pixels[243U * TEST_HIRES_WIDTH + 203U] == 78);
	assert_block(screen, 49, 60, 3);
	assert_block(screen, 50, 59, 3);
	assert_block(screen, 51, 60, 3);
	assert_block(screen, 50, 61, 3);
	screen->sprite.sprite_left = 0;
	screen->sprite.sprite_raster_left = 0;
	screen->sprite.sprite_right = TEST_WIDTH;
	screen->sprite.sprite_raster_right = TEST_WIDTH;
	screen->sprite.sprite_top = 0;
	screen->sprite.sprite_bottom = TEST_HEIGHT;
}

static void test_resource_ranges(struct TEST_SURFACE *screen, struct TEST_SURFACE *window)
{
	draw_detail(window, 10, 20);
	draw_detail(window, 12, 20);
	/* Replacing one byte in an overlapping paragraph alias must clear only
	 * that cell, preserving detail in the neighbouring live sprite pixels. */
	legacy_u16 offset = pixel_offset(window, 10, 20);
	hires_forget_range(window->base + offset, 1);
	window->base[offset] = 9;
	raster_pixel(screen, 30, 40, window, 10, 20, SHAPE2D_RASTER_COPY, NULL);
	assert_block(screen, 30, 40, 9);
	raster_pixel(screen, 32, 40, window, 12, 20, SHAPE2D_RASTER_COPY, NULL);
	assert_detail(screen, 32, 40, 255, 0);
	/* A captured resource shape can start inside its owning allocation. */
	hires_forget_range(window->base - 32, TEST_BYTES + 64);
	window->base[pixel_offset(window, 12, 20)] = 7;
	raster_pixel(screen, 32, 40, window, 12, 20, SHAPE2D_RASTER_COPY, NULL);
	assert_block(screen, 32, 40, 7);
	assert_block(screen, 30, 40, 9);
}

static void test_lifetime(struct TEST_SURFACE *screen, struct TEST_SURFACE *window)
{
	draw_detail(window, 10, 20);
	hires_forget(window->base);
	raster_pixel(screen, 30, 40, window, 10, 20, SHAPE2D_RASTER_COPY, NULL);
	assert_block(screen, 30, 40, 3);
	draw_detail(screen, 30, 40);
	assert_detail(screen, 30, 40, 255, 0);
	hires_set_enabled(0);
	assert(!hires_enabled());
	legacy_s32 width = 0;
	legacy_s32 height = 0;
	const legacy_u8 *pixels = hires_framebuffer(screen->base, &width, &height);
	assert(pixels == screen->base);
	assert(width == TEST_WIDTH);
	assert(height == TEST_HEIGHT);
	hires_set_enabled(1);
	assert(hires_enabled());
	assert_block(screen, 30, 40, 3);
}

static void depth_pixel(legacy_s32 x, legacy_s32 y, legacy_f64 inverse_z, legacy_u16 family,
						legacy_s32 attached, legacy_u8 color)
{
	if (hires_depth_test(x, y, inverse_z, family, attached)) {
		hires_pixel(x, y, color);
	}
}

static void test_crossing_depths(struct TEST_SURFACE *screen)
{
	for (legacy_u32 reverse = 0; reverse < 2; reverse++) {
		assert(hires_begin(&screen->sprite));
		hires_depth_begin(100, 121, 100, 101);
		for (legacy_u32 pass = 0; pass < 2; pass++) {
			legacy_u32 surface = pass ^ reverse;
			for (legacy_s32 x = 100; x < 121; x++) {
				legacy_f64 depth = (surface == 0 ? x - 90 : 130 - x) / 1000.0;
				depth_pixel(x, 100, depth, (legacy_u16)(surface + 1), 0, (legacy_u8)(surface + 40));
			}
		}
		hires_end();
		const legacy_u8 *pixels = get_framebuffer(screen);
		for (legacy_s32 x = 100; x < 121; x++) {
			if (x != 110) {
				assert(pixels[100 * TEST_HIRES_WIDTH + x] == (x < 110 ? 41 : 40));
			}
		}
		/* At equal depth the later submitted surface keeps painter order. */
		assert(pixels[100 * TEST_HIRES_WIDTH + 110] == (reverse ? 40 : 41));
	}
}

static void test_depth_overlay_families(struct TEST_SURFACE *screen)
{
	assert(hires_begin(&screen->sprite));
	hires_depth_begin(200, 203, 200, 201);
	depth_pixel(200, 200, 0.02, 1, 0, 50);
	depth_pixel(200, 200, 0.01, 1, 1, 51);
	depth_pixel(200, 200, 0.015, 2, 0, 52);
	/* The decal is visible and retains the parent's nearer occlusion depth. */
	assert(get_framebuffer(screen)[200 * TEST_HIRES_WIDTH + 200] == 51);

	depth_pixel(201, 200, 0.02, 1, 0, 50);
	depth_pixel(201, 200, 0.03, 2, 0, 52);
	depth_pixel(201, 200, 0.01, 1, 1, 51);
	assert(get_framebuffer(screen)[200 * TEST_HIRES_WIDTH + 201] == 52);

	assert(hires_depth_test(202, 200, 0.02, 1, 0));
	assert(hires_depth_test(202, 200, 0.02 * (1 - FLT_EPSILON), 2, 0));
	assert(!hires_depth_test(202, 200, 0.02 * (1 - 16 * FLT_EPSILON), 3, 0));
	assert(!hires_depth_test(202, 200, 0, 1, 0));
	assert(!hires_depth_test(202, 200, -1, 1, 0));
	assert(!hires_depth_test(202, 200, 1, 0, 0));
	hires_end();
}

static void test_depth_shape_bounds(struct TEST_SURFACE *screen)
{
	struct SPRITE clipped = screen->sprite;
	clipped.sprite_raster_left = 50;
	clipped.sprite_raster_right = 51;
	clipped.sprite_top = 60;
	clipped.sprite_bottom = 61;
	assert(hires_begin(&clipped));
	hires_depth_begin(-100, TEST_HIRES_WIDTH + 100, -100, TEST_HIRES_HEIGHT + 100);
	assert(!hires_depth_test(199, 240, 1, 1, 0));
	assert(!hires_depth_test(204, 240, 1, 1, 0));
	assert(!hires_depth_test(200, 239, 1, 1, 0));
	assert(!hires_depth_test(200, 244, 1, 1, 0));
	assert(hires_depth_test(200, 240, 1, 1, 0));
	assert(hires_depth_test(201, 241, 1, 1, 0));

	/* A new shape starts its own depth order while leaving other bounds alone. */
	hires_depth_begin(201, 202, 241, 242);
	assert(hires_depth_test(201, 241, 0.001, 2, 0));
	assert(!hires_depth_test(200, 240, 2, 2, 0));
	hires_depth_begin(200, 201, 240, 241);
	assert(hires_depth_test(200, 240, 0.001, 2, 0));
	hires_depth_begin(205, 206, 240, 241);
	assert(!hires_depth_test(200, 240, 2, 2, 0));
	assert(!hires_depth_test(205, 240, 2, 2, 0));
	hires_end();
}

static void test_depth_lifetime(struct TEST_SURFACE *screen)
{
	hires_depth_begin(0, TEST_HIRES_WIDTH, 0, TEST_HIRES_HEIGHT);
	assert(!hires_depth_test(0, 0, 1, 1, 0));
	assert(hires_begin(&screen->sprite));
	assert(!hires_depth_test(0, 0, 1, 1, 0));
	hires_depth_begin(0, 1, 0, 1);
	assert(hires_depth_test(0, 0, 1, 1, 0));
	hires_end();
	assert(!hires_depth_test(0, 0, 1, 1, 0));
	assert(hires_begin(&screen->sprite));
	assert(!hires_depth_test(0, 0, 1, 1, 0));
	hires_depth_begin(0, 1, 0, 1);
	assert(hires_depth_test(0, 0, 0.001, 2, 0));
	hires_set_enabled(0);
	assert(!hires_depth_test(0, 0, 1, 1, 0));
	hires_depth_begin(0, 1, 0, 1);
	assert(!hires_begin(&screen->sprite));
	hires_set_enabled(1);
	assert(hires_begin(&screen->sprite));
	assert(!hires_depth_test(0, 0, 1, 1, 0));
	hires_depth_begin(0, 1, 0, 1);
	assert(hires_depth_test(0, 0, 0.001, 2, 0));
	hires_shutdown();
	assert(!hires_depth_test(0, 0, 1, 1, 0));
}

int main(void)
{
	struct TEST_SURFACE screen;
	struct TEST_SURFACE window;
	setup_surface(&screen, 0xA000, 0);
	setup_surface(&window, 0x2000, SHAPE2D_HEADER_SIZE);
	assert(!hires_enabled());
	hires_set_enabled(1);
	assert(hires_enabled());
	legacy_u32 generation = hires_generation();
	test_detail_and_overlays(&screen, &window);
	assert(hires_generation() != generation);
	test_saved_background(&screen, &window);
	test_clipping(&screen);
	test_resource_ranges(&screen, &window);
	test_lifetime(&screen, &window);
	test_crossing_depths(&screen);
	test_depth_overlay_families(&screen);
	test_depth_shape_bounds(&screen);
	test_depth_lifetime(&screen);
	hires_shutdown();
	puts("SDL3 high-resolution composition tests passed.");
	return 0;
}
