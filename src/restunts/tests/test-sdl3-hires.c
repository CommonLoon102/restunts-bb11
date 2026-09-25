#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../c/hires.h"
#include "../c/render_workers.h"
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
	assert(!hires_shadow_begin());
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

static void test_argb_composition(struct TEST_SURFACE *screen, struct TEST_SURFACE *window)
{
	legacy_u32 palette[256];
	legacy_u8 mapping[256];
	for (legacy_u32 index = 0; index < 256; index++) {
		palette[index] = 0xFF000000U | index * 0x010101U;
		mapping[index] = (legacy_u8)index;
	}
	palette[15] = 0xFFFFFFFFU;
	assert(hires_begin_argb(&window->sprite));
	hires_argb_pixel(40, 80, 0xFF123456U);
	hires_argb_pixel(41, 80, 0x80FF0000U);
	hires_end();
	raster_pixel(screen, 30, 40, window, 10, 20, SHAPE2D_RASTER_COPY, NULL);
	legacy_u32 offset = 160 * TEST_HIRES_WIDTH + 120;
	const legacy_u32 *pixels = hires_framebuffer_argb(screen->base, palette);
	assert(pixels != NULL && pixels[offset] == 0xFF123456U);
	assert(pixels[offset + 1] == 0xFF810101U);
	assert(pixels[offset + 2] == palette[3]);
	/* Mouse save/restore and forward overlapping copies preserve all samples. */
	raster_pixel(window, 15, 25, screen, 30, 40, SHAPE2D_RASTER_COPY, NULL);
	write_pixel(screen, 30, 40, 3);
	assert(hires_framebuffer_argb(screen->base, palette) == NULL);
	raster_pixel(screen, 30, 40, window, 15, 25, SHAPE2D_RASTER_COPY, NULL);
	hires_raster(screen->base, 40 * 320 + 31, screen->base, 40 * 320 + 30, 2, SHAPE2D_RASTER_COPY,
				 NULL);
	pixels = hires_framebuffer_argb(screen->base, palette);
	assert(pixels[offset] == pixels[offset + 4] && pixels[offset] == pixels[offset + 8]);
	mapping[3] = 255;
	raster_pixel(screen, 30, 40, window, 15, 25, SHAPE2D_RASTER_MAP, mapping);
	assert(hires_framebuffer_argb(screen->base, palette)[offset] == 0xFF123456U);
	mapping[3] = 3;
	raster_pixel(screen, 30, 40, window, 15, 25, SHAPE2D_RASTER_MAP, mapping);
	assert(hires_framebuffer_argb(screen->base, palette)[offset] == 0xFF123456U);
	write_pixel(window, 11, 20, 255);
	raster_pixel(screen, 30, 40, window, 11, 20, SHAPE2D_RASTER_AND, NULL);
	write_pixel(window, 11, 20, 0);
	raster_pixel(screen, 30, 40, window, 11, 20, SHAPE2D_RASTER_OR, NULL);
	assert(hires_framebuffer_argb(screen->base, palette)[offset] == 0xFF123456U);
	mapping[3] = 42;
	raster_pixel(screen, 30, 40, window, 15, 25, SHAPE2D_RASTER_MAP, mapping);
	assert(hires_framebuffer_argb(screen->base, palette)[offset] == palette[42]);
	/* Indexed high-resolution replacement affects just one full-color sample. */
	assert(hires_begin(&screen->sprite));
	hires_pixel(124, 160, 42);
	hires_end();
	pixels = hires_framebuffer_argb(screen->base, palette);
	assert(pixels[offset + 4] == palette[42]);
	assert(pixels[offset + 5] == 0xFF810101U);
	palette[15] = 0xFF000000U;
	assert(hires_framebuffer_argb(screen->base, palette)[offset + 8] == 0xFF000000U);
	palette[15] = 0xFFFFFFFFU;
	hires_forget_range(screen->base + 40 * 320 + 31, 2);
	assert(hires_framebuffer_argb(screen->base, palette) == NULL);
}

static void test_logical_pixel_fill(void)
{
	struct TEST_SURFACE screen;
	struct TEST_SURFACE window;
	setup_surface(&screen, 0x5000, 0);
	setup_surface(&window, 0x6000, SHAPE2D_HEADER_SIZE);
	write_pixel(&window, 40, 60, 9);
	legacy_u32 palette[256];
	for (legacy_u32 index = 0; index < 256; index++) {
		palette[index] = 0xFF000000U | index * 0x010101U;
	}
	palette[15] = 0xFFFFFFFFU;
	assert(hires_begin_argb(&window.sprite));
	for (legacy_s32 y = 240; y < 244; y++) {
		for (legacy_s32 x = 160; x < 164; x++) {
			hires_argb_pixel(x, y, 0xFF123456U);
		}
	}
	hires_argb_pixel(164, 240, 0xFF654321U);
	hires_end();
	raster_pixel(&screen, 50, 60, &window, 40, 60, SHAPE2D_RASTER_COPY, NULL);
	raster_pixel(&screen, 51, 60, &window, 41, 60, SHAPE2D_RASTER_COPY, NULL);
	legacy_u32 offset = 240 * TEST_HIRES_WIDTH + 200;
	assert(hires_framebuffer_argb(screen.base, palette)[offset] == 0xFF123456U);
	/* Logical fills obey the active clip and do nothing outside a drawing pass. */
	hires_fill_pixel(40, 60, 99);
	struct SPRITE clipped = window.sprite;
	clipped.sprite_raster_left = 40;
	clipped.sprite_raster_right = 41;
	clipped.sprite_top = 60;
	clipped.sprite_bottom = 61;
	assert(hires_begin(&clipped));
	hires_fill_pixel(39, 60, 99);
	hires_fill_pixel(41, 60, 99);
	hires_fill_pixel(40, 59, 99);
	hires_fill_pixel(40, 61, 99);
	hires_fill_pixel(-1, -1, 99);
	hires_fill_pixel(TEST_WIDTH, TEST_HEIGHT, 99);
	hires_fill_pixel(40, 60, 77);
	hires_end();
	hires_fill_pixel(40, 60, 99);
	raster_pixel(&screen, 50, 60, &window, 40, 60, SHAPE2D_RASTER_COPY, NULL);
	raster_pixel(&screen, 51, 60, &window, 41, 60, SHAPE2D_RASTER_COPY, NULL);
	assert_block(&screen, 50, 60, 77);
	raster_pixel(&screen, 49, 60, &window, 39, 60, SHAPE2D_RASTER_COPY, NULL);
	raster_pixel(&screen, 50, 59, &window, 40, 59, SHAPE2D_RASTER_COPY, NULL);
	raster_pixel(&screen, 50, 61, &window, 40, 61, SHAPE2D_RASTER_COPY, NULL);
	assert_block(&screen, 49, 60, 3);
	assert_block(&screen, 50, 59, 3);
	assert_block(&screen, 50, 61, 3);
	const legacy_u32 *pixels = hires_framebuffer_argb(screen.base, palette);
	assert(pixels != NULL);
	for (legacy_u32 row = 0; row < TEST_SCALE; row++) {
		for (legacy_u32 column = 0; column < TEST_SCALE; column++) {
			assert(pixels[offset + row * TEST_HIRES_WIDTH + column] == palette[77]);
		}
	}
	assert(pixels[offset + TEST_SCALE] == 0xFF654321U);
	/* Removing the last overlay retires ARGB composition. The nonzero shape
	 * header offset and ordinary sprite copies retain the filled samples. */
	assert(hires_begin(&window.sprite));
	hires_fill_pixel(41, 60, 78);
	hires_fill_pixel(41, 60, 78);
	hires_end();
	assert(hires_framebuffer_argb(window.base, palette) == NULL);
	raster_pixel(&screen, 51, 60, &window, 41, 60, SHAPE2D_RASTER_COPY, NULL);
	assert(hires_framebuffer_argb(screen.base, palette) == NULL);
	assert_block(&screen, 51, 60, 78);
	for (legacy_u32 index = 0; index < TEST_BYTES; index++) {
		assert(window.base[window.first_pixel + index] == (index == 60 * TEST_WIDTH + 40 ? 9 : 3));
	}
	assert(screen.base[pixel_offset(&screen, 50, 60)] == 9);
	hires_forget(screen.base);
	hires_forget(window.base);
}

static void test_raster_target_aliases(void)
{
	struct TEST_SURFACE screen;
	setup_surface(&screen, 0x7000, 0);
	screen.sprite.sprite_raster_right = 2;
	screen.sprite.sprite_bottom = 2;
	struct HIRES_RASTER_TARGET target;
	assert(!hires_raster_prepare(&target));
	/* Distinct rows remain safe when a row wraps across the 64 KiB boundary. */
	LEGACY_WRITE_U16_LE(screen.lines, 65535U);
	LEGACY_WRITE_U16_LE(screen.lines + 2, 100U);
	assert(hires_begin(&screen.sprite));
	assert(hires_raster_prepare(&target));
	hires_end();
	/* Both complete aliases and partial overlaps through wrapping are unsafe. */
	LEGACY_WRITE_U16_LE(screen.lines + 2, 0U);
	assert(hires_begin(&screen.sprite));
	assert(!hires_raster_prepare(&target));
	hires_end();
	LEGACY_WRITE_U16_LE(screen.lines + 2, 65535U);
	assert(hires_begin(&screen.sprite));
	assert(!hires_raster_prepare(&target));
	hires_end();
	/* A reversed row table does not imply aliasing. */
	LEGACY_WRITE_U16_LE(screen.lines, 100U);
	LEGACY_WRITE_U16_LE(screen.lines + 2, 0U);
	assert(hires_begin(&screen.sprite));
	assert(hires_raster_prepare(&target));
	hires_end();
	hires_forget(screen.base);
}

static void raster_band_job(void *opaque, legacy_s32 job)
{
	struct HIRES_RASTER_CONTEXT *context = (struct HIRES_RASTER_CONTEXT *)opaque + job;
	/* Deliberately visit outside both clip and job bounds. Every write must
	 * still belong to exactly one complete legacy cell in this job. */
	for (legacy_s32 y = 239; y <= 248; y++) {
		for (legacy_s32 x = 159; x <= 176; x++) {
			hires_raster_pixel(context, x, y, 9);
			if (!hires_raster_depth_test(context, x, y, 0.02, 1, HIRES_DEPTH_SURFACE)) {
				continue;
			}
			hires_raster_pixel(context, x, y, 10);
			assert(hires_raster_depth_test(context, x, y, 0.01, 1, HIRES_DEPTH_ATTACHED));
			hires_raster_pixel(context, x, y, 11);
			assert(!hires_raster_depth_test(context, x, y, 0.015, 2, HIRES_DEPTH_SURFACE));
			assert(hires_raster_depth_test(context, x, y, 0.04, 1, HIRES_DEPTH_ORDERED));
			hires_raster_pixel(context, x, y, 12);
			assert(!hires_raster_depth_test(context, x, y, 0.03, 2, HIRES_DEPTH_SURFACE));
			assert(hires_raster_depth_test(context, x, y, 0.04, 3, HIRES_DEPTH_SURFACE));
			hires_raster_pixel(context, x, y, 13);
			assert(!hires_raster_depth_test(context, x, y, 0, 3, HIRES_DEPTH_SURFACE));
			assert(!hires_raster_depth_test(context, x, y, 1, 0, HIRES_DEPTH_SURFACE));
		}
	}
}

static void test_raster_bands(void)
{
	struct TEST_SURFACE screen;
	setup_surface(&screen, 0x7000, 0);
	legacy_u32 palette[256];
	for (legacy_u32 index = 0; index < 256; index++) {
		palette[index] = 0xFF000000U | index * 0x010101U;
	}
	palette[15] = 0xFFFFFFFFU;
	assert(hires_begin_argb(&screen.sprite));
	for (legacy_s32 y = 240; y < 248; y++) {
		for (legacy_s32 x = 160; x < 176; x++) {
			hires_argb_pixel(x, y, 0xFF123456U);
		}
	}
	/* A partial cell must retain its other ARGB samples until overwritten. */
	hires_argb_pixel(176, 240, 0xFF654321U);
	hires_argb_pixel(177, 240, 0xFF654321U);
	hires_end();
	struct SPRITE clipped = screen.sprite;
	clipped.sprite_raster_left = 40;
	clipped.sprite_raster_right = 44;
	clipped.sprite_top = 60;
	clipped.sprite_bottom = 62;
	assert(hires_begin(&clipped));
	hires_depth_begin(0, HIRES_WIDTH, 0, HIRES_HEIGHT);
	struct HIRES_RASTER_TARGET target;
	assert(hires_raster_prepare(&target));
	struct HIRES_RASTER_CONTEXT contexts[2] = {{&target, 240, 244, 0}, {&target, 244, 248, 0}};
	render_workers_run(2, raster_band_job, contexts);
	assert(contexts[0].cleared_argb_cells == 4);
	assert(contexts[1].cleared_argb_cells == 4);
	hires_raster_finish(&target, contexts[0].cleared_argb_cells + contexts[1].cleared_argb_cells);
	hires_end();
	for (legacy_u32 y = 60; y < 62; y++) {
		for (legacy_u32 x = 40; x < 44; x++) {
			assert_block(&screen, x, y, 13);
		}
	}
	assert_block(&screen, 39, 60, 3);
	assert_block(&screen, 40, 59, 3);
	assert_block(&screen, 40, 62, 3);
	legacy_u32 sample = 240 * HIRES_WIDTH + 176;
	const legacy_u32 *pixels = hires_framebuffer_argb(screen.base, palette);
	assert(pixels != NULL && pixels[sample] == 0xFF654321U);
	assert(hires_begin(&screen.sprite));
	hires_depth_begin(0, HIRES_WIDTH, 0, HIRES_HEIGHT);
	assert(hires_raster_prepare(&target));
	struct HIRES_RASTER_CONTEXT context = {&target, 240, 244, 0};
	hires_raster_pixel(&context, 176, 240, 42);
	assert(context.cleared_argb_cells == 0);
	hires_raster_pixel(&context, 177, 240, 43);
	hires_raster_pixel(&context, 177, 240, 43);
	assert(context.cleared_argb_cells == 1);
	hires_raster_finish(&target, context.cleared_argb_cells);
	hires_end();
	assert(hires_framebuffer_argb(screen.base, palette) == NULL);
	hires_forget(screen.base);
}

/* Shared fixtures for comparing batched spans with the per-pixel renderer. */
#define TEST_SPAN_SEGMENT 0x7000U
#define TEST_PALETTE_SIZE (LEGACY_U8_MAX + 1U)
#define TEST_BACKGROUND_COLOR 3U
#define TEST_SPAN_INITIAL_COLOR 17U
#define TEST_SPAN_COLOR 43U
#define TEST_SPAN_ALTERNATE_COLOR 201U
#define TEST_SPAN_INITIAL_ARGB 0x83123456U
#define TEST_SPAN_ARGB_CELL_PERIOD 2
#define TEST_SPAN_SAME_FAMILY 3U
#define TEST_SPAN_OTHER_FAMILY 7U
#define TEST_SPAN_NAN_BITS 0x7FC12345U
#define TEST_SPAN_INFINITY_BITS 0x7F800000U
#define TEST_SPAN_DEPTH 0.01
#define TEST_OPAQUE_ALPHA 0xFF000000U
#define TEST_GRAYSCALE_CHANNELS 0x010101U
#define TEST_WHITE_INDEX 15U
#define TEST_WHITE_ARGB 0xFFFFFFFFU

enum TEST_SPAN_BACKEND { TEST_SPAN_REFERENCE, TEST_SPAN_BATCHED, TEST_SPAN_BACKEND_COUNT };

/* Keep the oracle in the per-pixel C APIs, independent of span grouping. */
static void raster_span_reference(struct HIRES_RASTER_CONTEXT *context, legacy_s32 left,
								  legacy_s32 right, legacy_s32 y, legacy_f64 inverse_z,
								  legacy_f64 depth_step, legacy_u32 family, legacy_s32 depth_mode,
								  legacy_u16 color, legacy_u16 alternate, legacy_u16 pattern,
								  legacy_s32 paint_mode, legacy_s32 depth_test)
{
	const struct HIRES_RASTER_TARGET *target = context->target;
	for (legacy_s32 x = left; x < right; x++, inverse_z += depth_step) {
		if (x < target->left || x >= target->right || y < target->top || y >= target->bottom ||
			y < context->top || y >= context->bottom) {
			continue;
		}
		legacy_u8 sample_color = (legacy_u8)color;
		legacy_u32 bit =
			(HIRES_PATTERN_ROW_MASK - (y & HIRES_PATTERN_ROW_MASK)) * HIRES_PATTERN_WIDTH +
			HIRES_PATTERN_COLUMN_MASK - (x & HIRES_PATTERN_COLUMN_MASK);
		if (paint_mode != HIRES_PAINT_SOLID) {
			if ((pattern & (1U << bit)) != 0) {
				if (paint_mode == HIRES_PAINT_ALTERNATE) {
					sample_color = (legacy_u8)alternate;
				}
			} else if (paint_mode != HIRES_PAINT_ALTERNATE) {
				continue;
			}
		}
		if (!depth_test || hires_raster_depth_test(context, x, y, inverse_z, family, depth_mode)) {
			hires_raster_pixel(context, x, y, sample_color);
		}
	}
}

/* Construct IEEE-754 edge cases without the project's math.h shadowing the
 * system header. memcpy also keeps the exact NaN payload available to compare. */
static legacy_f32 raster_span_float_bits(legacy_u32 bits)
{
	legacy_f32 value;
	memcpy(&value, &bits, sizeof(value));
	return value;
}

static void test_raster_span_depth_edges(legacy_s32 paint_mode)
{
	enum {
		EDGE_LEFT = 40 * HIRES_SCALE,
		EDGE_TOP = 60 * HIRES_SCALE,
		EDGE_WIDTH = 16 * HIRES_SCALE,
		EDGE_CLIP_INSET = 2,
		EDGE_DEPTH_INSET = EDGE_CLIP_INSET + 1,
		EDGE_SPAN_OVERHANG = 1,
		EDGE_FAMILY_CYCLE = 4,
		EDGE_OTHER_FAMILY_SLOT = 2,
		EDGE_UNKNOWN_DEPTH_MODE = HIRES_DEPTH_ORDERED + 1,
		EDGE_DISABLED_DEPTH,
		EDGE_INVALID_FAMILY,
		EDGE_MODE_COUNT,
		EDGE_PATTERN = 0x5AA5,
		EDGE_DEPTH_EPSILON_UNITS = 4,
		EDGE_OVERFLOW_FACTOR = 2,
		EDGE_OVERFLOW_STEP_DIVISOR = 16,
		EDGE_SUBNORMAL_DIVISOR = 1024
	};
	const legacy_f64 zero_crossing_depth = 0.000001;
	const legacy_f64 zero_crossing_step = 0.0000001;
	const legacy_f64 sub_ulp_step = 0.000000000000000001;
	const legacy_u16 color = TEST_SPAN_COLOR | (1U << LEGACY_BYTE_BITS);
	const legacy_u16 alternate = TEST_SPAN_ALTERNATE_COLOR | (1U << (LEGACY_BYTE_BITS + 1U));
	const legacy_f32 not_a_number = raster_span_float_bits(TEST_SPAN_NAN_BITS);
	const legacy_f32 infinity = raster_span_float_bits(TEST_SPAN_INFINITY_BITS);
	struct TEST_SURFACE screen;
	setup_surface(&screen, TEST_SPAN_SEGMENT, 0);
	legacy_u32 palette[TEST_PALETTE_SIZE];
	for (legacy_u32 index = 0; index < TEST_PALETTE_SIZE; index++) {
		palette[index] = TEST_OPAQUE_ALPHA | index * TEST_GRAYSCALE_CHANNELS;
	}
	const struct {
		legacy_f64 inverse_z, step;
	} cases[] = {{TEST_SPAN_DEPTH, 0},
				 {1.0 + DBL_EPSILON, 0},
				 {1.0 - DBL_EPSILON, 0},
				 {0.0, 0},
				 {-0.0, -0.0},
				 {-zero_crossing_depth, zero_crossing_step},
				 {zero_crossing_depth, -zero_crossing_step},
				 {FLT_MAX, 0},
				 {(legacy_f64)FLT_MAX * EDGE_OVERFLOW_FACTOR, 0},
				 {FLT_MAX, -(legacy_f64)FLT_MAX / EDGE_OVERFLOW_STEP_DIVISOR},
				 {(legacy_f64)FLT_MAX * EDGE_OVERFLOW_FACTOR,
				  -(legacy_f64)FLT_MAX / EDGE_OVERFLOW_STEP_DIVISOR},
				 {not_a_number, 0},
				 {infinity, 0},
				 {-infinity, 0},
				 {TEST_SPAN_DEPTH, not_a_number},
				 {TEST_SPAN_DEPTH, infinity},
				 {TEST_SPAN_DEPTH, -infinity},
				 {(legacy_f64)FLT_MIN / EDGE_SUBNORMAL_DIVISOR, 0},
				 {DBL_MIN, 0},
				 {TEST_SPAN_DEPTH, sub_ulp_step},
				 {TEST_SPAN_DEPTH, -sub_ulp_step}};
	const legacy_f32 previous[] = {
		0.0f,
		-0.0f,
		infinity,
		-infinity,
		not_a_number,
		-FLT_MAX,
		FLT_MAX,
		(legacy_f32)TEST_SPAN_DEPTH,
		1.0f,
		1.0f + FLT_EPSILON,
		1.0f - FLT_EPSILON,
		FLT_MIN,
		-FLT_MIN,
		(legacy_f32)TEST_SPAN_DEPTH * (1 + EDGE_DEPTH_EPSILON_UNITS * FLT_EPSILON),
		(legacy_f32)TEST_SPAN_DEPTH * (1 - EDGE_DEPTH_EPSILON_UNITS * FLT_EPSILON),
		(legacy_f32)TEST_SPAN_DEPTH * (1 - (EDGE_DEPTH_EPSILON_UNITS + 1) * FLT_EPSILON)};
	enum {
		EDGE_ROWS = sizeof(cases) / sizeof(cases[0]),
		EDGE_PREVIOUS_COUNT = sizeof(previous) / sizeof(previous[0])
	};
	legacy_u8 reference_pixels[EDGE_ROWS][EDGE_WIDTH];
	legacy_u32 reference_argb[EDGE_ROWS][EDGE_WIDTH];
	legacy_f32 reference_depth[EDGE_ROWS][EDGE_WIDTH];
	legacy_u32 reference_family[EDGE_ROWS][EDGE_WIDTH];
	legacy_u32 reference_cleared = 0;
	/* Include an unknown attached-like mode, disabled depth, and invalid
	 * family zero. Each old depth occurs with several families. */
	for (legacy_s32 mode = HIRES_DEPTH_SURFACE; mode < EDGE_MODE_COUNT; mode++) {
		for (legacy_s32 span = TEST_SPAN_REFERENCE; span < TEST_SPAN_BACKEND_COUNT; span++) {
			hires_forget(screen.base);
			assert(hires_begin_argb(&screen.sprite));
			hires_depth_begin(0, HIRES_WIDTH, 0, HIRES_HEIGHT);
			struct HIRES_RASTER_TARGET target;
			assert(hires_raster_prepare(&target));
			for (legacy_s32 row = 0; row < EDGE_ROWS; row++) {
				for (legacy_s32 column = 0; column < EDGE_WIDTH; column++) {
					legacy_s32 x = EDGE_LEFT + column;
					legacy_s32 y = EDGE_TOP + row;
					hires_pixel(x, y, TEST_SPAN_INITIAL_COLOR);
					if (x / HIRES_SCALE % TEST_SPAN_ARGB_CELL_PERIOD == 0) {
						hires_argb_pixel(x, y, TEST_SPAN_INITIAL_ARGB);
					}
					size_t index = (size_t)y * HIRES_WIDTH + x;
					target.inverse_depth[index] = previous[column % EDGE_PREVIOUS_COUNT];
					legacy_s32 owner = (column + column / EDGE_PREVIOUS_COUNT) % EDGE_FAMILY_CYCLE;
					target.depth_family[index] = owner == 0 ? HIRES_DEPTH_FAMILY_NONE
												 : owner == EDGE_OTHER_FAMILY_SLOT
													 ? TEST_SPAN_OTHER_FAMILY
													 : TEST_SPAN_SAME_FAMILY;
				}
			}
			target.left = EDGE_LEFT + EDGE_CLIP_INSET;
			target.right = EDGE_LEFT + EDGE_WIDTH - EDGE_CLIP_INSET;
			target.depth_left = EDGE_LEFT + EDGE_DEPTH_INSET;
			target.depth_right = EDGE_LEFT + EDGE_WIDTH - EDGE_DEPTH_INSET;
			struct HIRES_RASTER_CONTEXT context = {&target, EDGE_TOP, EDGE_TOP + EDGE_ROWS, 0};
			for (legacy_s32 row = 0; row < EDGE_ROWS; row++) {
				legacy_u32 family =
					mode == EDGE_INVALID_FAMILY ? HIRES_DEPTH_FAMILY_NONE : TEST_SPAN_SAME_FAMILY;
				/* Both positive and negative interpolation cross exceptional
				 * depths, including inside four-lane groups and patterned holes. */
				if (span == TEST_SPAN_BATCHED) {
					hires_raster_span(&context, EDGE_LEFT - EDGE_SPAN_OVERHANG,
									  EDGE_LEFT + EDGE_WIDTH + EDGE_SPAN_OVERHANG, EDGE_TOP + row,
									  cases[row].inverse_z, cases[row].step, family, mode, color,
									  alternate, EDGE_PATTERN, paint_mode,
									  mode != EDGE_DISABLED_DEPTH);
				} else {
					raster_span_reference(&context, EDGE_LEFT - EDGE_SPAN_OVERHANG,
										  EDGE_LEFT + EDGE_WIDTH + EDGE_SPAN_OVERHANG,
										  EDGE_TOP + row, cases[row].inverse_z, cases[row].step,
										  family, mode, color, alternate, EDGE_PATTERN, paint_mode,
										  mode != EDGE_DISABLED_DEPTH);
				}
			}
			hires_raster_finish(&target, context.cleared_argb_cells);
			hires_end();
			const legacy_u8 *indexed = get_framebuffer(&screen);
			const legacy_u32 *argb = hires_framebuffer_argb(screen.base, palette);
			assert(argb != NULL);
			if (span == TEST_SPAN_REFERENCE) {
				reference_cleared = context.cleared_argb_cells;
			} else {
				assert(reference_cleared == context.cleared_argb_cells);
			}
			for (legacy_s32 row = 0; row < EDGE_ROWS; row++) {
				for (legacy_s32 column = 0; column < EDGE_WIDTH; column++) {
					size_t index = (size_t)(row + EDGE_TOP) * HIRES_WIDTH + column + EDGE_LEFT;
					if (span == TEST_SPAN_REFERENCE) {
						reference_pixels[row][column] = indexed[index];
						reference_argb[row][column] = argb[index];
						reference_depth[row][column] = target.inverse_depth[index];
						reference_family[row][column] = target.depth_family[index];
					} else {
						assert(reference_pixels[row][column] == indexed[index]);
						assert(reference_argb[row][column] == argb[index]);
						/* Compare stored bits so unchanged NaNs and signed zero
						 * cannot silently change through masked vector stores. */
						assert(memcmp(&reference_depth[row][column], &target.inverse_depth[index],
									  sizeof(legacy_f32)) == 0);
						assert(reference_family[row][column] == target.depth_family[index]);
					}
				}
			}
		}
	}
	hires_forget(screen.base);
}

static void test_raster_span_argb_retirement(void)
{
	enum {
		RETIREMENT_CELL_LEFT = 40,
		RETIREMENT_CELL_TOP = 60,
		RETIREMENT_CELL_COUNT = 8,
		RETIREMENT_LEFT = RETIREMENT_CELL_LEFT * HIRES_SCALE,
		RETIREMENT_TOP = RETIREMENT_CELL_TOP * HIRES_SCALE,
		RETIREMENT_RIGHT = RETIREMENT_LEFT + RETIREMENT_CELL_COUNT * HIRES_SCALE,
		RETIREMENT_BOTTOM = RETIREMENT_TOP + HIRES_SCALE,
		RETIREMENT_REPEATS = 2,
		RETIREMENT_EVEN_COLUMNS = 0xAAAA,
		RETIREMENT_ODD_COLUMNS = 0x5555
	};
	const legacy_u16 patterns[] = {RETIREMENT_EVEN_COLUMNS, RETIREMENT_ODD_COLUMNS};
	const legacy_s32 pass_count = RETIREMENT_REPEATS * (sizeof(patterns) / sizeof(patterns[0]));
	const legacy_f32 not_a_number = raster_span_float_bits(TEST_SPAN_NAN_BITS);
	struct TEST_SURFACE screen;
	setup_surface(&screen, TEST_SPAN_SEGMENT, 0);
	for (legacy_s32 span = TEST_SPAN_REFERENCE; span < TEST_SPAN_BACKEND_COUNT; span++) {
		hires_forget(screen.base);
		assert(hires_begin_argb(&screen.sprite));
		for (legacy_s32 y = RETIREMENT_TOP; y < RETIREMENT_BOTTOM; y++) {
			for (legacy_s32 x = RETIREMENT_LEFT; x < RETIREMENT_RIGHT; x++) {
				hires_pixel(x, y, TEST_SPAN_INITIAL_COLOR);
				hires_argb_pixel(x, y, TEST_SPAN_INITIAL_ARGB);
			}
		}
		struct HIRES_RASTER_TARGET target;
		assert(hires_raster_prepare(&target));
		/* Unconditional paint must work without any allocated depth arrays. */
		target.inverse_depth = NULL;
		target.depth_family = NULL;
		struct HIRES_RASTER_CONTEXT context = {&target, RETIREMENT_TOP, RETIREMENT_BOTTOM, 0};
		for (legacy_s32 pass = 0; pass < pass_count; pass++) {
			legacy_u16 pattern = patterns[pass / RETIREMENT_REPEATS];
			for (legacy_s32 y = RETIREMENT_TOP; y < RETIREMENT_BOTTOM; y++) {
				if (span == TEST_SPAN_BATCHED) {
					hires_raster_span(&context, RETIREMENT_LEFT, RETIREMENT_RIGHT, y, not_a_number,
									  not_a_number, HIRES_DEPTH_FAMILY_NONE, HIRES_DEPTH_SURFACE,
									  TEST_SPAN_COLOR, TEST_SPAN_ALTERNATE_COLOR, pattern,
									  HIRES_PAINT_PATTERN, 0);
				} else {
					raster_span_reference(
						&context, RETIREMENT_LEFT, RETIREMENT_RIGHT, y, not_a_number, not_a_number,
						HIRES_DEPTH_FAMILY_NONE, HIRES_DEPTH_SURFACE, TEST_SPAN_COLOR,
						TEST_SPAN_ALTERNATE_COLOR, pattern, HIRES_PAINT_PATTERN, 0);
				}
			}
			/* Repeated writes to already indexed samples cannot retire a cell
			 * twice; complementary holes retire it only after the fourth row. */
			assert(context.cleared_argb_cells ==
				   (pass < RETIREMENT_REPEATS ? 0 : RETIREMENT_CELL_COUNT));
		}
		hires_raster_finish(&target, context.cleared_argb_cells);
		hires_end();
		for (legacy_s32 x = RETIREMENT_CELL_LEFT; x < RETIREMENT_CELL_LEFT + RETIREMENT_CELL_COUNT;
			 x++) {
			assert_block(&screen, x, RETIREMENT_CELL_TOP, TEST_SPAN_COLOR);
		}
	}
	hires_forget(screen.base);
}

/* Exercise clipped cell tails, every pattern nibble and whole-cell ARGB
 * retirement against the existing per-pixel API, including band boundaries. */
static void test_raster_spans(void)
{
	enum {
		SPAN_LEFT = 40 * HIRES_SCALE,
		SPAN_TOP = 60 * HIRES_SCALE,
		SPAN_WIDTH = 32 * HIRES_SCALE,
		SPAN_HEIGHT = 4 * HIRES_SCALE,
		SPAN_RIGHT = SPAN_LEFT + SPAN_WIDTH,
		SPAN_BOTTOM = SPAN_TOP + SPAN_HEIGHT,
		SPAN_HORIZONTAL_MARGIN = 2 * HIRES_SCALE,
		SPAN_VERTICAL_MARGIN = HIRES_SCALE,
		SPAN_REFERENCE_LEFT = SPAN_LEFT - SPAN_HORIZONTAL_MARGIN,
		SPAN_REFERENCE_TOP = SPAN_TOP - SPAN_VERTICAL_MARGIN,
		SPAN_REFERENCE_WIDTH = SPAN_WIDTH + 2 * SPAN_HORIZONTAL_MARGIN,
		SPAN_REFERENCE_HEIGHT = SPAN_HEIGHT + 2 * SPAN_VERTICAL_MARGIN,
		SPAN_DEPTH_HORIZONTAL_INSET = HIRES_SCALE - 1,
		SPAN_DEPTH_VERTICAL_INSET = HIRES_SCALE / 2,
		SPAN_BAND_TOP = SPAN_TOP + HIRES_SCALE,
		SPAN_DEPTH_VARIANTS = 7,
		SPAN_FAMILY_VARIANTS = 3,
		SPAN_NIBBLE_BITS = 4,
		SPAN_NIBBLE_VARIANTS = 1U << SPAN_NIBBLE_BITS,
		SPAN_NIBBLE_REPEAT = 0x1111
	};
	const legacy_f32 previous_depth_step = 0.001f;
	const legacy_f64 row_depth_step = 0.005;
	const legacy_f64 pixel_depth_step = 0.00001;
	struct TEST_SURFACE screen;
	setup_surface(&screen, TEST_SPAN_SEGMENT, 0);
	legacy_u32 palette[TEST_PALETTE_SIZE];
	for (legacy_u32 index = 0; index < TEST_PALETTE_SIZE; index++) {
		palette[index] = TEST_OPAQUE_ALPHA | index * TEST_GRAYSCALE_CHANNELS;
	}
	palette[TEST_WHITE_INDEX] = TEST_WHITE_ARGB;
	legacy_u8 reference_pixels[SPAN_REFERENCE_HEIGHT][SPAN_REFERENCE_WIDTH];
	legacy_u32 reference_argb[SPAN_REFERENCE_HEIGHT][SPAN_REFERENCE_WIDTH];
	legacy_f32 reference_depth[SPAN_REFERENCE_HEIGHT][SPAN_REFERENCE_WIDTH];
	legacy_u32 reference_family[SPAN_REFERENCE_HEIGHT][SPAN_REFERENCE_WIDTH];
	legacy_u32 reference_cleared = 0;
	for (legacy_s32 depth_mode = HIRES_DEPTH_SURFACE; depth_mode <= HIRES_DEPTH_ORDERED;
		 depth_mode++) {
		for (legacy_s32 paint_mode = HIRES_PAINT_SOLID; paint_mode <= HIRES_PAINT_ALTERNATE;
			 paint_mode++) {
			for (legacy_s32 span = TEST_SPAN_REFERENCE; span < TEST_SPAN_BACKEND_COUNT; span++) {
				hires_forget(screen.base);
				assert(hires_begin_argb(&screen.sprite));
				hires_depth_begin(0, HIRES_WIDTH, 0, HIRES_HEIGHT);
				struct HIRES_RASTER_TARGET target;
				assert(hires_raster_prepare(&target));
				for (legacy_s32 y = SPAN_REFERENCE_TOP;
					 y < SPAN_REFERENCE_TOP + SPAN_REFERENCE_HEIGHT; y++) {
					for (legacy_s32 x = SPAN_REFERENCE_LEFT;
						 x < SPAN_REFERENCE_LEFT + SPAN_REFERENCE_WIDTH; x++) {
						hires_pixel(x, y, TEST_SPAN_INITIAL_COLOR);
						if (x / HIRES_SCALE % TEST_SPAN_ARGB_CELL_PERIOD == 0) {
							hires_argb_pixel(x, y, TEST_SPAN_INITIAL_ARGB);
						}
						size_t pixel = (size_t)y * HIRES_WIDTH + x;
						target.inverse_depth[pixel] =
							(legacy_f32)TEST_SPAN_DEPTH +
							(x % SPAN_DEPTH_VARIANTS) * previous_depth_step;
						target.depth_family[pixel] =
							x % SPAN_FAMILY_VARIANTS == 0	? HIRES_DEPTH_FAMILY_NONE
							: x % SPAN_FAMILY_VARIANTS == 1 ? TEST_SPAN_SAME_FAMILY
															: TEST_SPAN_OTHER_FAMILY;
					}
				}
				target.left = SPAN_LEFT;
				target.right = SPAN_RIGHT;
				target.top = SPAN_TOP;
				target.bottom = SPAN_BOTTOM;
				target.depth_left = SPAN_LEFT + SPAN_DEPTH_HORIZONTAL_INSET;
				target.depth_right = SPAN_RIGHT - SPAN_DEPTH_HORIZONTAL_INSET;
				target.depth_top = SPAN_TOP + SPAN_DEPTH_VERTICAL_INSET;
				target.depth_bottom = SPAN_BOTTOM - SPAN_DEPTH_VERTICAL_INSET;
				struct HIRES_RASTER_CONTEXT context = {&target, SPAN_BAND_TOP, SPAN_BOTTOM, 0};
				for (legacy_s32 y = SPAN_TOP - 1; y < SPAN_BOTTOM + 1; y++) {
					legacy_s32 left = SPAN_LEFT - (HIRES_SCALE - 1) + y % HIRES_SCALE;
					legacy_s32 right = SPAN_RIGHT + HIRES_SCALE - y % HIRES_SCALE;
					legacy_f64 inverse_z = row_depth_step + (y % HIRES_SCALE) * row_depth_step;
					legacy_f64 step =
						y % HIRES_PATTERN_HEIGHT ? pixel_depth_step : -pixel_depth_step;
					legacy_u16 pattern =
						(legacy_u16)((y % SPAN_NIBBLE_VARIANTS) * SPAN_NIBBLE_REPEAT);
					legacy_s32 depth_test = y % HIRES_SCALE != 0;
					if (span == TEST_SPAN_BATCHED) {
						hires_raster_span(&context, left, right, y, inverse_z, step,
										  TEST_SPAN_SAME_FAMILY, depth_mode, TEST_SPAN_COLOR,
										  TEST_SPAN_ALTERNATE_COLOR, pattern, paint_mode,
										  depth_test);
					} else {
						raster_span_reference(&context, left, right, y, inverse_z, step,
											  TEST_SPAN_SAME_FAMILY, depth_mode, TEST_SPAN_COLOR,
											  TEST_SPAN_ALTERNATE_COLOR, pattern, paint_mode,
											  depth_test);
					}
				}
				hires_raster_finish(&target, context.cleared_argb_cells);
				hires_end();
				const legacy_u8 *pixels = get_framebuffer(&screen);
				const legacy_u32 *argb = hires_framebuffer_argb(screen.base, palette);
				assert(argb != NULL);
				if (span == TEST_SPAN_REFERENCE) {
					reference_cleared = context.cleared_argb_cells;
				} else {
					assert(reference_cleared == context.cleared_argb_cells);
				}
				for (legacy_s32 row = 0; row < SPAN_REFERENCE_HEIGHT; row++) {
					size_t pixel =
						(size_t)(row + SPAN_REFERENCE_TOP) * HIRES_WIDTH + SPAN_REFERENCE_LEFT;
					if (span == TEST_SPAN_REFERENCE) {
						memcpy(reference_pixels[row], pixels + pixel,
							   sizeof(reference_pixels[row]));
						memcpy(reference_argb[row], argb + pixel, sizeof(reference_argb[row]));
						memcpy(reference_depth[row], target.inverse_depth + pixel,
							   sizeof(reference_depth[row]));
						memcpy(reference_family[row], target.depth_family + pixel,
							   sizeof(reference_family[row]));
					} else {
						assert(memcmp(reference_pixels[row], pixels + pixel,
									  sizeof(reference_pixels[row])) == 0);
						assert(memcmp(reference_argb[row], argb + pixel,
									  sizeof(reference_argb[row])) == 0);
						assert(memcmp(reference_depth[row], target.inverse_depth + pixel,
									  sizeof(reference_depth[row])) == 0);
						assert(memcmp(reference_family[row], target.depth_family + pixel,
									  sizeof(reference_family[row])) == 0);
					}
				}
			}
		}
	}
	hires_forget(screen.base);
}

static void test_logical_pixel_write(void)
{
	enum {
		WRITE_CELL_X = 50,
		WRITE_CELL_Y = 60,
		WRITE_LEFT = WRITE_CELL_X * HIRES_SCALE,
		WRITE_TOP = WRITE_CELL_Y * HIRES_SCALE,
		WRITE_RIGHT = WRITE_LEFT + HIRES_SCALE,
		WRITE_BOTTOM = WRITE_TOP + HIRES_SCALE
	};
	const legacy_u32 cell_argb = 0xFF123456U;
	const legacy_u32 neighbour_argb = 0xFF654321U;
	struct TEST_SURFACE screen;
	setup_surface(&screen, TEST_SPAN_SEGMENT, 0);
	legacy_u8 samples[HIRES_SCALE * HIRES_SCALE];
	for (legacy_u32 sample = 0; sample < sizeof(samples); sample++) {
		samples[sample] = (legacy_u8)(sample + TEST_SPAN_INITIAL_COLOR);
	}
	legacy_u32 palette[TEST_PALETTE_SIZE] = {0};
	palette[TEST_WHITE_INDEX] = TEST_WHITE_ARGB;
	assert(hires_begin_argb(&screen.sprite));
	for (legacy_s32 y = WRITE_TOP; y < WRITE_BOTTOM; y++) {
		for (legacy_s32 x = WRITE_LEFT; x < WRITE_RIGHT; x++) {
			hires_argb_pixel(x, y, cell_argb);
		}
	}
	hires_argb_pixel(WRITE_RIGHT, WRITE_TOP, neighbour_argb);
	hires_end();
	struct SPRITE clip = screen.sprite;
	clip.sprite_raster_left = WRITE_CELL_X;
	clip.sprite_raster_right = WRITE_CELL_X + 1;
	clip.sprite_top = WRITE_CELL_Y;
	clip.sprite_bottom = WRITE_CELL_Y + 1;
	assert(hires_begin(&clip));
	hires_write_pixel(WRITE_CELL_X, WRITE_CELL_Y, samples);
	hires_write_pixel(WRITE_CELL_X - 1, WRITE_CELL_Y, samples);
	hires_write_pixel(WRITE_CELL_X + 1, WRITE_CELL_Y, samples);
	hires_write_pixel(WRITE_CELL_X, WRITE_CELL_Y - 1, samples);
	hires_write_pixel(WRITE_CELL_X, WRITE_CELL_Y + 1, samples);
	hires_write_pixel(-1, WRITE_CELL_Y, samples);
	hires_write_pixel(TEST_WIDTH, WRITE_CELL_Y, samples);
	hires_end();
	const legacy_u8 *pixels = get_framebuffer(&screen);
	for (legacy_u32 sample = 0; sample < sizeof(samples); sample++) {
		assert(pixels[(WRITE_TOP + sample / HIRES_SCALE) * HIRES_WIDTH + WRITE_LEFT +
					  sample % HIRES_SCALE] == samples[sample]);
	}
	assert(screen.base[pixel_offset(&screen, WRITE_CELL_X, WRITE_CELL_Y)] == TEST_BACKGROUND_COLOR);
	assert_block(&screen, WRITE_CELL_X - 1, WRITE_CELL_Y, TEST_BACKGROUND_COLOR);
	assert_block(&screen, WRITE_CELL_X + 1, WRITE_CELL_Y, TEST_BACKGROUND_COLOR);
	assert_block(&screen, WRITE_CELL_X, WRITE_CELL_Y - 1, TEST_BACKGROUND_COLOR);
	assert_block(&screen, WRITE_CELL_X, WRITE_CELL_Y + 1, TEST_BACKGROUND_COLOR);
	const legacy_u32 *argb = hires_framebuffer_argb(screen.base, palette);
	assert(argb != NULL && argb[WRITE_TOP * HIRES_WIDTH + WRITE_RIGHT] == neighbour_argb);
	write_pixel(&screen, WRITE_CELL_X + 1, WRITE_CELL_Y, TEST_BACKGROUND_COLOR);
	assert(hires_framebuffer_argb(screen.base, palette) == NULL);
	hires_forget(screen.base);
}

static void test_shadow_composition(void)
{
	struct TEST_SURFACE screen;
	setup_surface(&screen, 0x8000, 0);
	legacy_u32 palette[256] = {0};
	palette[3] = 0xFF90C0F0U;
	palette[15] = 0xFFFFFFFFU;
	assert(!hires_shadow_begin());
	hires_shadow_pixel(200, 240, 255);
	struct SPRITE clipped = screen.sprite;
	clipped.sprite_raster_left = 50;
	clipped.sprite_raster_right = 51;
	clipped.sprite_top = 60;
	clipped.sprite_bottom = 61;
	assert(hires_begin(&clipped));
	hires_depth_begin(200, 204, 240, 244);
	assert(hires_depth_test(200, 240, 0.02, 7, HIRES_DEPTH_SURFACE));
	struct HIRES_RASTER_TARGET target;
	assert(hires_raster_prepare(&target));
	legacy_u32 offset = 240 * HIRES_WIDTH + 200;
	legacy_f32 depth = target.inverse_depth[offset];
	assert(hires_shadow_begin());
	assert(hires_shadow_begin());
	hires_argb_pixel(201, 240, 0xFF80C040U);
	hires_argb_pixel(202, 240, 0x80FF0000U);
	hires_shadow_pixel(200, 240, 85);
	hires_shadow_pixel(201, 240, 85);
	hires_shadow_pixel(202, 240, 85);
	hires_shadow_pixel(203, 240, 0);
	hires_shadow_pixel(199, 240, 255);
	hires_shadow_pixel(204, 240, 255);
	hires_shadow_pixel(200, 239, 255);
	hires_shadow_pixel(200, 244, 255);
	hires_shadow_pixel(-1, 240, 255);
	hires_shadow_pixel(HIRES_WIDTH, 240, 255);
	assert(target.inverse_depth[offset] == depth && target.depth_family[offset] == 7);
	assert(target.depth_left == 200 && target.depth_right == 204);
	assert(target.depth_top == 240 && target.depth_bottom == 244);
	assert(!hires_depth_test(200, 240, 0.01, 8, HIRES_DEPTH_SURFACE));
	hires_end();
	const legacy_u32 *pixels = hires_framebuffer_argb(screen.base, palette);
	assert(pixels != NULL && pixels[offset] == 0xFF6080A0U);
	assert(pixels[offset + 1] == 0xFF55802AU);
	assert(pixels[offset + 2] == 0xFF854050U);
	assert(pixels[offset + 3] == palette[3]);
	assert(pixels[offset - 1] == palette[3]);
	assert(pixels[offset + 4] == palette[3]);
	assert(pixels[offset - HIRES_WIDTH] == palette[3]);
	assert(pixels[offset + 4 * HIRES_WIDTH] == palette[3]);
	assert_block(&screen, 50, 60, 3);
	assert(hires_begin(&clipped));
	assert(hires_shadow_begin());
	hires_shadow_pixel(200, 240, 255);
	hires_end();
	assert(hires_framebuffer_argb(screen.base, palette)[offset] == 0xFF000000U);
	/* Ordinary painting must still clear the shadow and its ARGB bookkeeping. */
	write_pixel(&screen, 50, 60, 3);
	assert(hires_framebuffer_argb(screen.base, palette) == NULL);
	hires_forget(screen.base);
}

legacy_int main(void)
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
	test_argb_composition(&screen, &window);
	test_logical_pixel_fill();
	test_logical_pixel_write();
	test_raster_target_aliases();
	test_raster_bands();
	test_raster_spans();
	for (legacy_s32 paint_mode = HIRES_PAINT_SOLID; paint_mode <= HIRES_PAINT_ALTERNATE;
		 paint_mode++) {
		test_raster_span_depth_edges(paint_mode);
	}
	test_raster_span_argb_retirement();
	test_shadow_composition();
	test_depth_lifetime(&screen);
	hires_shutdown();
	puts("SDL3 high-resolution composition tests passed.");
	return 0;
}
