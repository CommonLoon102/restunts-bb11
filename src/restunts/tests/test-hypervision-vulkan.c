#include <assert.h>
#include <float.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../c/hypervision.h"
#include "../c/platform.h"
#include "../c/render_workers.h"
#include "../c/shape2d.h"
#include "../c/shape2d_internal.h"

#define GPU_TEST_WIDTH (HIRES_WIDTH / HIRES_SCALE)
#define GPU_TEST_HEIGHT (HIRES_HEIGHT / HIRES_SCALE)
#define GPU_TEST_SCREEN_SEGMENT 0xA000U
#define GPU_TEST_BACKGROUND 3U
#define GPU_TEST_FIRST_COLOR 7U
#define GPU_TEST_SECOND_COLOR 8U
#define GPU_TEST_DETAIL_COLOR 9U
#define GPU_TEST_ALTERNATE_COLOR 10U
#define GPU_TEST_FIRST_FAMILY 0xF123ABCDU
#define GPU_TEST_SECOND_FAMILY 0x81234567U
#define GPU_TEST_FAR_DEPTH 0.001
#define GPU_TEST_MIDDLE_DEPTH 0.002
#define GPU_TEST_NEAR_DEPTH 0.004
#define GPU_TEST_DEPTH_TOLERANCE 0.000001
#define GPU_TEST_PATTERN 0xAAAAU
#define GPU_TEST_SUBNORMAL_DEPTH (FLT_MIN / 2.0)
#define GPU_TEST_RECTANGLE_VERTICES 4U
#define GPU_TEST_TOUCHING_VERTICES 6U
#define GPU_TEST_CONCAVE_VERTICES 8U
#define GPU_TEST_CIRCLE_VERTICES 8U
#define GPU_TEST_REDUNDANT_VERTICES (GPU_TEST_CONCAVE_VERTICES + 2U)
#define GPU_TEST_TAU 6.28318530717958647693
#define GPU_TEST_BORDER 8
#define GPU_TEST_COVERAGE_TOLERANCE 1.0
#define GPU_TEST_OVERLAY 0xFF123456U
#define GPU_TEST_OPAQUE_ALPHA 0xFF000000U
#define GPU_TEST_GRAYSCALE_CHANNELS 0x010101U
#define GPU_TEST_WHITE_INDEX 15U
#define GPU_TEST_WHITE_COLOR 0xFFFFFFFFU
#define GPU_TEST_SHADOW_ALPHA 128U
#define GPU_TEST_SKIP 77
#define GPU_TEST_OPTION_ARGC 2

static struct SPRITE sprite;
static struct HIRES_RASTER_TARGET target;
static legacy_u8 rows[GPU_TEST_HEIGHT * LEGACY_WORD_BYTES];
static legacy_u8 *screen;
static legacy_u32 palette[LEGACY_U8_MAX + 1U];
static enum HYPERVISION_BACKEND expected_backend = HYPERVISION_BACKEND_VULKAN;

static void begin_frame(legacy_s32 scale, legacy_s32 clipped, legacy_s32 overlay)
{
	hires_shutdown();
	hires_set_enabled(1);
	hires_set_render_scale(scale);
	memset(screen, GPU_TEST_BACKGROUND, GPU_TEST_WIDTH * GPU_TEST_HEIGHT);
	legacy_s32 width = hires_render_width(), height = hires_render_height();
	if (overlay) {
		assert(hires_begin_argb(&sprite));
		hires_argb_pixel(1, 1, GPU_TEST_OVERLAY);
		hires_argb_pixel(width / 2, height / 2, GPU_TEST_OVERLAY);
		hires_argb_pixel(width / 4, height / 4, GPU_TEST_OVERLAY);
		hires_argb_pixel(width / 2, height / 4, GPU_TEST_OVERLAY);
		hires_argb_pixel(width / 2 + 1, height / 4, GPU_TEST_OVERLAY);
		hires_end();
	}
	struct SPRITE clip = sprite;
	if (clipped) {
		clip.sprite_raster_left = clip.sprite_top = GPU_TEST_BORDER;
		clip.sprite_raster_right = GPU_TEST_WIDTH - GPU_TEST_BORDER;
		clip.sprite_bottom = GPU_TEST_HEIGHT - GPU_TEST_BORDER;
	}
	assert(hires_begin(&clip));
	hires_depth_begin(0, width, 0, height);
	assert(hires_raster_prepare(&target));
	hypervision_begin(NULL);
}

static const legacy_u8 *end_frame(void)
{
	(void)hypervision_end(&target, 0);
	/* A runtime failure is an error even when CPU fallback produces good pixels. */
	assert(hypervision_backend() == expected_backend);
	hires_end();
	legacy_s32 width, height;
	const legacy_u8 *pixels = hires_framebuffer(screen, &width, &height);
	assert(pixels != NULL && width == target.width && height == target.height);
	return pixels;
}

static struct HYPERVISION_MATERIAL material(legacy_u16 color, legacy_u32 family)
{
	struct HYPERVISION_MATERIAL result = {family, color, 0, 0, HIRES_PAINT_SOLID, 0};
	return result;
}

static void rectangle(legacy_f64 left, legacy_f64 right, legacy_f64 top, legacy_f64 bottom,
					  legacy_f64 left_depth, legacy_f64 right_depth,
					  const struct HYPERVISION_MATERIAL *paint)
{
	const struct HYPERVISION_VERTEX vertices[GPU_TEST_RECTANGLE_VERTICES] = {
		{left, top, left_depth},
		{right, top, right_depth},
		{right, bottom, right_depth},
		{left, bottom, left_depth}};
	hypervision_polygon(vertices, GPU_TEST_RECTANGLE_VERTICES, paint);
}

static void assert_depth(legacy_s32 x, legacy_s32 y, legacy_f64 depth, legacy_u32 family)
{
	size_t index = (size_t)y * target.width + x;
	assert(SDL_fabs(target.inverse_depth[index] - depth) < GPU_TEST_DEPTH_TOLERANCE);
	assert(target.depth_family[index] == family);
}

static void test_crossing_depth(legacy_s32 scale)
{
	const struct HYPERVISION_MATERIAL first = material(GPU_TEST_FIRST_COLOR, GPU_TEST_FIRST_FAMILY);
	const struct HYPERVISION_MATERIAL second =
		material(GPU_TEST_SECOND_COLOR, GPU_TEST_SECOND_FAMILY);
	for (legacy_s32 reverse = 0; reverse <= 1; reverse++) {
		begin_frame(scale, 0, 0);
		for (legacy_s32 order = 0; order <= 1; order++) {
			if ((order ^ reverse) == 0) {
				rectangle(0, target.width, 0, target.height, GPU_TEST_FAR_DEPTH,
						  GPU_TEST_NEAR_DEPTH, &first);
			} else {
				rectangle(0, target.width, 0, target.height, GPU_TEST_NEAR_DEPTH,
						  GPU_TEST_FAR_DEPTH, &second);
			}
		}
		const legacy_u8 *pixels = end_frame();
		for (legacy_s32 y = 0; y < target.height; y++) {
			for (legacy_s32 x = 0; x < target.width; x++) {
				assert(pixels[(size_t)y * target.width + x] ==
					   (x < target.width / 2 ? GPU_TEST_SECOND_COLOR : GPU_TEST_FIRST_COLOR));
			}
		}
	}
}

/* Ray crossings form an independent coverage oracle. Only samples within one
 * pixel of an edge may differ because GPU fill conventions and precision vary. */
static void assert_polygon_coverage(const legacy_u8 *pixels,
									const struct HYPERVISION_VERTEX *vertices, legacy_u32 count)
{
	for (legacy_s32 y = 0; y < target.height; y++) {
		for (legacy_s32 x = 0; x < target.width; x++) {
			legacy_f64 px = x + HIRES_SAMPLE_CENTER_OFFSET;
			legacy_f64 py = y + HIRES_SAMPLE_CENTER_OFFSET;
			legacy_s32 inside = 0, boundary = 0;
			for (legacy_u32 first = 0, last = count - 1; first < count; last = first++) {
				const struct HYPERVISION_VERTEX *a = &vertices[first], *b = &vertices[last];
				legacy_f64 dx = b->x - a->x, dy = b->y - a->y;
				legacy_f64 length_squared = dx * dx + dy * dy;
				legacy_f64 along = length_squared != 0
									   ? ((px - a->x) * dx + (py - a->y) * dy) / length_squared
									   : 0;
				if (along < 0) {
					along = 0;
				} else if (along > 1) {
					along = 1;
				}
				legacy_f64 edge_x = px - a->x - along * dx;
				legacy_f64 edge_y = py - a->y - along * dy;
				if (edge_x * edge_x + edge_y * edge_y <=
					GPU_TEST_COVERAGE_TOLERANCE * GPU_TEST_COVERAGE_TOLERANCE) {
					boundary = 1;
				}
				if (((a->y > py) != (b->y > py)) && px < a->x + (py - a->y) * dx / dy) {
					inside = !inside;
				}
			}
			if (!boundary) {
				assert(pixels[(size_t)y * target.width + x] ==
					   (inside ? GPU_TEST_FIRST_COLOR : GPU_TEST_BACKGROUND));
			}
		}
	}
}

static void test_polygon_coverage(legacy_s32 scale)
{
	const struct HYPERVISION_MATERIAL paint = material(GPU_TEST_FIRST_COLOR, GPU_TEST_FIRST_FAMILY);
	begin_frame(scale, 0, 0);
	legacy_f64 left = target.width / 8, right = target.width * 7 / 8;
	legacy_f64 top = target.height / 8, bottom = target.height * 7 / 8;
	const struct HYPERVISION_VERTEX concave[GPU_TEST_CONCAVE_VERTICES] = {
		{left, top, GPU_TEST_MIDDLE_DEPTH},
		{right, top, GPU_TEST_MIDDLE_DEPTH},
		{right, bottom, GPU_TEST_MIDDLE_DEPTH},
		{target.width * 5 / 8, bottom, GPU_TEST_MIDDLE_DEPTH},
		{target.width * 5 / 8, target.height / 2, GPU_TEST_MIDDLE_DEPTH},
		{target.width * 3 / 8, target.height / 2, GPU_TEST_MIDDLE_DEPTH},
		{target.width * 3 / 8, bottom, GPU_TEST_MIDDLE_DEPTH},
		{left, bottom, GPU_TEST_MIDDLE_DEPTH}};
	hypervision_polygon(concave, GPU_TEST_CONCAVE_VERTICES, &paint);
	assert_polygon_coverage(end_frame(), concave, GPU_TEST_CONCAVE_VERTICES);
	begin_frame(scale, 0, 0);
	struct HYPERVISION_VERTEX reversed[GPU_TEST_CONCAVE_VERTICES];
	for (legacy_u32 vertex = 0; vertex < GPU_TEST_CONCAVE_VERTICES; vertex++) {
		reversed[vertex] = concave[GPU_TEST_CONCAVE_VERTICES - vertex - 1U];
	}
	hypervision_polygon(reversed, GPU_TEST_CONCAVE_VERTICES, &paint);
	assert_polygon_coverage(end_frame(), concave, GPU_TEST_CONCAVE_VERTICES);
	begin_frame(scale, 0, 0);
	struct HYPERVISION_VERTEX redundant[GPU_TEST_REDUNDANT_VERTICES];
	redundant[0] = concave[0];
	redundant[1] = (struct HYPERVISION_VERTEX){(left + right) / 2, top, GPU_TEST_MIDDLE_DEPTH};
	redundant[2] = redundant[3] = concave[1];
	for (legacy_u32 vertex = 2; vertex < GPU_TEST_CONCAVE_VERTICES; vertex++) {
		redundant[vertex + 2U] = concave[vertex];
	}
	hypervision_polygon(redundant, GPU_TEST_REDUNDANT_VERTICES, &paint);
	assert_polygon_coverage(end_frame(), concave, GPU_TEST_CONCAVE_VERTICES);
	begin_frame(scale, 0, 0);
	struct HYPERVISION_VERTEX circle[GPU_TEST_CIRCLE_VERTICES];
	legacy_f64 radius = target.height / 3;
	for (legacy_u32 vertex = 0; vertex < GPU_TEST_CIRCLE_VERTICES; vertex++) {
		legacy_f64 angle = vertex * GPU_TEST_TAU / GPU_TEST_CIRCLE_VERTICES;
		circle[vertex] = (struct HYPERVISION_VERTEX){target.width / 2 + radius * SDL_cos(angle),
													 target.height / 2 + radius * SDL_sin(angle),
													 GPU_TEST_MIDDLE_DEPTH};
	}
	hypervision_polygon(circle, GPU_TEST_CIRCLE_VERTICES, &paint);
	assert_polygon_coverage(end_frame(), circle, GPU_TEST_CIRCLE_VERTICES);
}

static void test_crossing_and_touching_coverage(legacy_s32 scale)
{
	const struct HYPERVISION_MATERIAL paint = material(GPU_TEST_FIRST_COLOR, GPU_TEST_FIRST_FAMILY);
	begin_frame(scale, 0, 0);
	legacy_f64 left = target.width / 4, right = target.width * 3 / 4;
	legacy_f64 top = target.height / 4, bottom = target.height * 3 / 4;
	const struct HYPERVISION_VERTEX bowtie[GPU_TEST_RECTANGLE_VERTICES] = {
		{left, top, GPU_TEST_MIDDLE_DEPTH},
		{right, bottom, GPU_TEST_MIDDLE_DEPTH},
		{right, top, GPU_TEST_MIDDLE_DEPTH},
		{left, bottom, GPU_TEST_MIDDLE_DEPTH}};
	hypervision_polygon(bowtie, GPU_TEST_RECTANGLE_VERTICES, &paint);
	assert_polygon_coverage(end_frame(), bowtie, GPU_TEST_RECTANGLE_VERTICES);
	begin_frame(scale, 0, 0);
	const struct HYPERVISION_VERTEX touching[GPU_TEST_TOUCHING_VERTICES] = {
		{target.width / 2, target.height / 2, GPU_TEST_MIDDLE_DEPTH},
		{left, top, GPU_TEST_MIDDLE_DEPTH},
		{right, top, GPU_TEST_MIDDLE_DEPTH},
		{target.width / 2, target.height / 2, GPU_TEST_MIDDLE_DEPTH},
		{right, bottom, GPU_TEST_MIDDLE_DEPTH},
		{left, bottom, GPU_TEST_MIDDLE_DEPTH}};
	hypervision_polygon(touching, GPU_TEST_TOUCHING_VERTICES, &paint);
	assert_polygon_coverage(end_frame(), touching, GPU_TEST_TOUCHING_VERTICES);
}

static void test_background_depth(legacy_s32 scale)
{
	struct HYPERVISION_MATERIAL background = material(GPU_TEST_FIRST_COLOR, GPU_TEST_FIRST_FAMILY);
	background.flags = HYPERVISION_BACKGROUND;
	const struct HYPERVISION_MATERIAL foreground =
		material(GPU_TEST_SECOND_COLOR, GPU_TEST_SECOND_FAMILY);
	for (legacy_s32 reverse = 0; reverse <= 1; reverse++) {
		begin_frame(scale, 0, 0);
		for (legacy_s32 order = 0; order <= 1; order++) {
			if ((order ^ reverse) == 0) {
				rectangle(0, target.width, 0, target.height, GPU_TEST_NEAR_DEPTH,
						  GPU_TEST_NEAR_DEPTH, &background);
			} else {
				rectangle(target.width / 4, target.width * 3 / 4, target.height / 4,
						  target.height * 3 / 4, GPU_TEST_FAR_DEPTH, GPU_TEST_FAR_DEPTH,
						  &foreground);
			}
		}
		const legacy_u8 *pixels = end_frame();
		assert(pixels[0] == GPU_TEST_FIRST_COLOR);
		assert_depth(0, 0, 0, 0);
		assert(pixels[(size_t)(target.height / 2) * target.width + target.width / 2] ==
			   GPU_TEST_SECOND_COLOR);
		assert_depth(target.width / 2, target.height / 2, GPU_TEST_FAR_DEPTH,
					 GPU_TEST_SECOND_FAMILY);
	}
}

static void test_patterns(legacy_s32 scale)
{
	const struct HYPERVISION_MATERIAL base = material(GPU_TEST_FIRST_COLOR, GPU_TEST_FIRST_FAMILY);
	struct HYPERVISION_MATERIAL paint = material(GPU_TEST_SECOND_COLOR, GPU_TEST_SECOND_FAMILY);
	paint.pattern = GPU_TEST_PATTERN;
	paint.alternate = GPU_TEST_ALTERNATE_COLOR;
	for (legacy_s32 mode = HIRES_PAINT_PATTERN; mode <= HIRES_PAINT_ALTERNATE; mode++) {
		begin_frame(scale, 0, 0);
		paint.mode = (legacy_u8)mode;
		rectangle(0, target.width, 0, target.height, GPU_TEST_FAR_DEPTH, GPU_TEST_FAR_DEPTH, &base);
		rectangle(0, target.width, 0, target.height, GPU_TEST_NEAR_DEPTH, GPU_TEST_NEAR_DEPTH,
				  &paint);
		const legacy_u8 *pixels = end_frame();
		for (legacy_s32 y = 0; y < target.height; y++) {
			for (legacy_s32 x = 0; x < target.width; x++) {
				legacy_s32 covered = (x & 1) == 0 || mode == HIRES_PAINT_ALTERNATE;
				legacy_u8 expected =
					mode == HIRES_PAINT_ALTERNATE
						? ((x & 1) == 0 ? GPU_TEST_ALTERNATE_COLOR : GPU_TEST_SECOND_COLOR)
						: ((x & 1) == 0 ? GPU_TEST_SECOND_COLOR : GPU_TEST_FIRST_COLOR);
				assert(pixels[(size_t)y * target.width + x] == expected);
				assert_depth(x, y, covered ? GPU_TEST_NEAR_DEPTH : GPU_TEST_FAR_DEPTH,
							 covered ? GPU_TEST_SECOND_FAMILY : GPU_TEST_FIRST_FAMILY);
			}
		}
	}
}

static void test_attached_and_ground(legacy_s32 scale)
{
	const struct HYPERVISION_MATERIAL support =
		material(GPU_TEST_FIRST_COLOR, GPU_TEST_FIRST_FAMILY);
	const struct HYPERVISION_MATERIAL nearer =
		material(GPU_TEST_SECOND_COLOR, GPU_TEST_SECOND_FAMILY);
	struct HYPERVISION_MATERIAL detail = material(GPU_TEST_DETAIL_COLOR, GPU_TEST_FIRST_FAMILY);
	detail.flags = HYPERVISION_DECAL;
	for (legacy_s32 reverse = 0; reverse <= 1; reverse++) {
		begin_frame(scale, 0, 0);
		for (legacy_s32 order = 0; order <= 1; order++) {
			if ((order ^ reverse) == 0) {
				rectangle(0, target.width * 3 / 4, 0, target.height, GPU_TEST_MIDDLE_DEPTH,
						  GPU_TEST_MIDDLE_DEPTH, &support);
			} else {
				rectangle(target.width / 4, target.width, target.height / 4, target.height * 3 / 4,
						  GPU_TEST_FAR_DEPTH, GPU_TEST_FAR_DEPTH, &detail);
			}
		}
		rectangle(target.width / 2, target.width * 5 / 8, 0, target.height, GPU_TEST_NEAR_DEPTH,
				  GPU_TEST_NEAR_DEPTH, &nearer);
		const legacy_u8 *pixels = end_frame();
		legacy_s32 y = target.height / 2, supported = target.width * 3 / 8;
		legacy_s32 blocked = target.width * 9 / 16, empty = target.width * 7 / 8;
		assert(pixels[(size_t)y * target.width + supported] == GPU_TEST_DETAIL_COLOR);
		assert(pixels[(size_t)y * target.width + blocked] == GPU_TEST_SECOND_COLOR);
		assert(pixels[(size_t)y * target.width + empty] == GPU_TEST_DETAIL_COLOR);
		assert_depth(supported, y, GPU_TEST_MIDDLE_DEPTH, GPU_TEST_FIRST_FAMILY);
		assert_depth(blocked, y, GPU_TEST_NEAR_DEPTH, GPU_TEST_SECOND_FAMILY);
		assert_depth(empty, y, GPU_TEST_FAR_DEPTH, GPU_TEST_FIRST_FAMILY);
	}
	begin_frame(scale, 0, 0);
	struct HIRES_DEPTH_PLANE ground = {GPU_TEST_MIDDLE_DEPTH / target.width, 0, GPU_TEST_FAR_DEPTH};
	hypervision_begin(&ground);
	detail.flags = HYPERVISION_GROUND_CLIP;
	detail.family = GPU_TEST_SECOND_FAMILY;
	rectangle(target.width * 7 / 8, target.width, 0, target.height, GPU_TEST_FAR_DEPTH,
			  GPU_TEST_FAR_DEPTH, &support);
	rectangle(0, target.width, 0, target.height, GPU_TEST_MIDDLE_DEPTH, GPU_TEST_MIDDLE_DEPTH,
			  &detail);
	const legacy_u8 *pixels = end_frame();
	legacy_s32 y = target.height / 2;
	assert(pixels[(size_t)y * target.width + target.width / 4] == GPU_TEST_DETAIL_COLOR);
	assert(pixels[(size_t)y * target.width + target.width * 3 / 4] == GPU_TEST_BACKGROUND);
	assert(pixels[(size_t)y * target.width + target.width * 15 / 16] == GPU_TEST_DETAIL_COLOR);
}

static void test_layered_attached_patterns(legacy_s32 scale)
{
	begin_frame(scale, 0, 0);
	const struct HYPERVISION_MATERIAL support =
		material(GPU_TEST_FIRST_COLOR, GPU_TEST_FIRST_FAMILY);
	struct HYPERVISION_MATERIAL solid = material(GPU_TEST_SECOND_COLOR, GPU_TEST_FIRST_FAMILY);
	struct HYPERVISION_MATERIAL patterned = material(GPU_TEST_DETAIL_COLOR, GPU_TEST_FIRST_FAMILY);
	solid.flags = patterned.flags = HYPERVISION_DECAL;
	patterned.mode = HIRES_PAINT_PATTERN;
	patterned.pattern = GPU_TEST_PATTERN;
	rectangle(0, target.width, 0, target.height, GPU_TEST_NEAR_DEPTH, GPU_TEST_NEAR_DEPTH,
			  &support);
	rectangle(0, target.width, 0, target.height, GPU_TEST_MIDDLE_DEPTH, GPU_TEST_MIDDLE_DEPTH,
			  &solid);
	rectangle(0, target.width, 0, target.height, GPU_TEST_FAR_DEPTH, GPU_TEST_FAR_DEPTH,
			  &patterned);
	const legacy_u8 *pixels = end_frame();
	for (legacy_s32 y = 0; y < target.height; y++) {
		for (legacy_s32 x = 0; x < target.width; x++) {
			assert(pixels[(size_t)y * target.width + x] ==
				   ((x & 1) == 0 ? GPU_TEST_DETAIL_COLOR : GPU_TEST_SECOND_COLOR));
			assert_depth(x, y, GPU_TEST_NEAR_DEPTH, GPU_TEST_FIRST_FAMILY);
		}
	}
}

static void test_clip_overlay_shadow(legacy_s32 scale)
{
	begin_frame(scale, 1, 1);
	const struct HYPERVISION_MATERIAL paint = material(GPU_TEST_FIRST_COLOR, GPU_TEST_FIRST_FAMILY);
	rectangle(0, target.width / 2 + 1, 0, target.height / 2, GPU_TEST_MIDDLE_DEPTH,
			  GPU_TEST_MIDDLE_DEPTH, &paint);
	const legacy_u8 *pixels = end_frame();
	for (legacy_s32 y = 0; y < target.height; y++) {
		for (legacy_s32 x = 0; x < target.width; x++) {
			legacy_s32 covered = x >= target.left && x <= target.width / 2 && y >= target.top &&
								 y < target.height / 2;
			assert(pixels[(size_t)y * target.width + x] ==
				   (covered ? GPU_TEST_FIRST_COLOR : GPU_TEST_BACKGROUND));
		}
	}
	const legacy_u32 *argb = hires_framebuffer_argb(screen, palette);
	assert(argb != NULL);
	legacy_s32 x = target.width / 4, y = target.height / 4;
	size_t covered = (size_t)y * target.width + x;
	size_t hole = (size_t)(target.height / 2) * target.width + target.width / 2;
	assert(argb[target.width + 1] == GPU_TEST_OVERLAY && argb[hole] == GPU_TEST_OVERLAY);
	assert(argb[covered] == palette[GPU_TEST_FIRST_COLOR]);
	size_t partial_cell = (size_t)(target.height / 4) * target.width + target.width / 2;
	assert(argb[partial_cell] == palette[GPU_TEST_FIRST_COLOR]);
	assert(argb[partial_cell + 1] == GPU_TEST_OVERLAY);
	assert_depth(x, y, GPU_TEST_MIDDLE_DEPTH, GPU_TEST_FIRST_FAMILY);
	assert(hires_begin(&sprite));
	assert(hires_shadow_begin());
	hires_shadow_pixel(x, y, GPU_TEST_SHADOW_ALPHA);
	hires_end();
	argb = hires_framebuffer_argb(screen, palette);
	assert(argb[covered] != palette[GPU_TEST_FIRST_COLOR]);
	assert(argb[hole] == GPU_TEST_OVERLAY);
	assert_depth(x, y, GPU_TEST_MIDDLE_DEPTH, GPU_TEST_FIRST_FAMILY);
	for (size_t pixel = 0; pixel < GPU_TEST_WIDTH * GPU_TEST_HEIGHT; pixel++) {
		assert(screen[pixel] == GPU_TEST_BACKGROUND);
	}
}

static void test_toggle_and_reset(legacy_s32 scale)
{
	assert(hypervision_select_backend(HYPERVISION_BACKEND_CPU));
	expected_backend = HYPERVISION_BACKEND_CPU;
	begin_frame(scale, 0, 0);
	const struct HYPERVISION_MATERIAL paint = material(GPU_TEST_FIRST_COLOR, GPU_TEST_FIRST_FAMILY);
	rectangle(0, target.width, 0, target.height, GPU_TEST_MIDDLE_DEPTH, GPU_TEST_MIDDLE_DEPTH,
			  &paint);
	const legacy_u8 *pixels = end_frame();
	assert(pixels[0] == GPU_TEST_FIRST_COLOR);
	assert(hypervision_select_backend(HYPERVISION_BACKEND_VULKAN));
	expected_backend = HYPERVISION_BACKEND_VULKAN;
	begin_frame(scale, 0, 0);
	pixels = end_frame();
	for (size_t pixel = 0; pixel < (size_t)target.width * target.height; pixel++) {
		assert(pixels[pixel] == GPU_TEST_BACKGROUND);
	}
}

static void submit_unsupported_scene(void)
{
	const struct HYPERVISION_MATERIAL first = material(GPU_TEST_FIRST_COLOR, GPU_TEST_FIRST_FAMILY);
	const struct HYPERVISION_MATERIAL second =
		material(GPU_TEST_SECOND_COLOR, GPU_TEST_SECOND_FAMILY);
	rectangle(0, target.width / 2, 0, target.height, GPU_TEST_MIDDLE_DEPTH, GPU_TEST_MIDDLE_DEPTH,
			  &second);
	/* CPU depth storage supports this positive subnormal value. The GPU contract
	 * rejects it before submission because portable shader arithmetic may flush it. */
	rectangle(target.width / 2, target.width, 0, target.height, GPU_TEST_SUBNORMAL_DEPTH,
			  GPU_TEST_SUBNORMAL_DEPTH, &first);
}

static void test_explicit_fallback_and_recovery(void)
{
	assert(hypervision_select_backend(HYPERVISION_BACKEND_CPU));
	expected_backend = HYPERVISION_BACKEND_CPU;
	begin_frame(HIRES_MEDIUM_SCALE, 0, 0);
	submit_unsupported_scene();
	const legacy_u8 *pixels = end_frame();
	assert(pixels[0] == GPU_TEST_SECOND_COLOR);
	assert(pixels[target.width - 1] == GPU_TEST_FIRST_COLOR);
	size_t bytes = (size_t)target.width * target.height;
	legacy_u8 *reference = malloc(bytes);
	assert(reference != NULL);
	memcpy(reference, pixels, bytes);
	assert(hypervision_select_backend(HYPERVISION_BACKEND_VULKAN));
	begin_frame(HIRES_MEDIUM_SCALE, 0, 0);
	submit_unsupported_scene();
	/* This one explicitly tested unsupported depth must trigger fallback and
	 * redraw the entire frame, including earlier opaque geometry. */
	pixels = end_frame();
	assert(memcmp(reference, pixels, bytes) == 0);
	free(reference);
	assert(hypervision_select_backend(HYPERVISION_BACKEND_VULKAN));
	expected_backend = HYPERVISION_BACKEND_VULKAN;
	begin_frame(HIRES_MEDIUM_SCALE, 0, 0);
	const struct HYPERVISION_MATERIAL paint = material(GPU_TEST_FIRST_COLOR, GPU_TEST_FIRST_FAMILY);
	rectangle(0, target.width, 0, target.height, GPU_TEST_MIDDLE_DEPTH, GPU_TEST_MIDDLE_DEPTH,
			  &paint);
	pixels = end_frame();
	assert(pixels[0] == GPU_TEST_FIRST_COLOR);
	puts("Intentional unsupported-depth CPU fallback and Vulkan recovery passed.");
}

legacy_int main(legacy_int argc, legacy_char *argv[])
{
	legacy_s32 required = argc == GPU_TEST_OPTION_ARGC && strcmp(argv[1], "--require-vulkan") == 0;
	if (argc != 1 && !required) {
		fputs("Usage: test-hypervision-vulkan [--require-vulkan]\n", stderr);
		return EXIT_FAILURE;
	}
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "SDL video initialization failed: %s\n", SDL_GetError());
		return required ? EXIT_FAILURE : GPU_TEST_SKIP;
	}
	if (!hypervision_select_backend(HYPERVISION_BACKEND_VULKAN)) {
		fprintf(stderr, "Vulkan backend unavailable: %s\n", SDL_GetError());
		hypervision_shutdown();
		SDL_Quit();
		return required ? EXIT_FAILURE : GPU_TEST_SKIP;
	}
	printf("Vulkan device: %s; driver: %s; hardware accelerated: %d\n", hypervision_device_name(),
		   hypervision_driver_name(), hypervision_hardware_accelerated());
	screen = dos_memory_make_pointer(GPU_TEST_SCREEN_SEGMENT, 0);
	sprite.sprite_bitmapptr = (struct SHAPE2D *)screen;
	sprite.sprite_lineofs = rows;
	sprite.sprite_right = sprite.sprite_pitch = sprite.sprite_buffer_width = GPU_TEST_WIDTH;
	sprite.sprite_raster_right = GPU_TEST_WIDTH;
	sprite.sprite_bottom = GPU_TEST_HEIGHT;
	for (legacy_u32 row = 0; row < GPU_TEST_HEIGHT; row++) {
		LEGACY_WRITE_U16_LE(rows + row * LEGACY_WORD_BYTES, row * GPU_TEST_WIDTH);
	}
	for (legacy_u32 color = 0; color <= LEGACY_U8_MAX; color++) {
		palette[color] = GPU_TEST_OPAQUE_ALPHA | color * GPU_TEST_GRAYSCALE_CHANNELS;
	}
	palette[GPU_TEST_WHITE_INDEX] = GPU_TEST_WHITE_COLOR;
	const legacy_s32 scales[] = {HIRES_MINIMUM_SCALE, HIRES_SCALE, HIRES_MEDIUM_SCALE, HIRES_SCALE};
	for (size_t index = 0; index < sizeof(scales) / sizeof(scales[0]); index++) {
		test_crossing_depth(scales[index]);
		test_polygon_coverage(scales[index]);
		test_crossing_and_touching_coverage(scales[index]);
		test_background_depth(scales[index]);
		test_patterns(scales[index]);
		test_attached_and_ground(scales[index]);
		test_layered_attached_patterns(scales[index]);
		test_clip_overlay_shadow(scales[index]);
		test_toggle_and_reset(scales[index]);
	}
	test_explicit_fallback_and_recovery();
	hypervision_shutdown();
	hires_shutdown();
	render_workers_shutdown();
	SDL_Quit();
	puts("Vulkan depth, polygon coverage, materials, imports, shadows and resizing passed.");
	return EXIT_SUCCESS;
}
