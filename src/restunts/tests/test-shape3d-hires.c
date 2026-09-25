#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <SDL3/SDL_stdinc.h>
#include "../c/externs.h"
#include "../c/hires.h"
#include "../c/platform.h"
#include "../c/projection.h"
#include "../c/shape2d.h"
#include "../c/shape3d_hires.h"
#include "../c/shape3d_internal.h"

#undef memcpy

static struct SPRITE target;
static legacy_u8 rows[200 * 2];
static legacy_u8 *screen;
static struct RECTANGLE bounds;

static void reset_target(void)
{
	hires_shutdown();
	memset(screen, 3, 320 * 200);
	hires_set_enabled(1);
	shape3d_hires_reset();
	assert(hires_begin(&target));
}

static const legacy_u8 *pixels(void)
{
	legacy_s32 width;
	legacy_s32 height;
	const legacy_u8 *result = hires_framebuffer(screen, &width, &height);
	assert(width == HIRES_WIDTH && height == HIRES_HEIGHT);
	return result;
}

static legacy_u32 count_color(legacy_u8 color)
{
	const legacy_u8 *image = pixels();
	legacy_u32 count = 0;
	for (legacy_u32 index = 0; index < HIRES_WIDTH * HIRES_HEIGHT; index++) {
		count += image[index] == color;
		if (image[index] == color) {
			legacy_s32 x = (index % HIRES_WIDTH) / HIRES_SCALE;
			legacy_s32 y = (index / HIRES_WIDTH) / HIRES_SCALE;
			assert(x >= bounds.left && x < bounds.right);
			assert(y >= bounds.top && y < bounds.bottom);
		}
	}
	return count;
}

static void queue_flagged(legacy_u8 type, legacy_u32 count,
						  const struct SHAPE3D_HIRES_VECTOR *vertices, legacy_u16 flags)
{
	legacy_u8 indices[10];
	assert(count <= sizeof(indices));
	for (legacy_u32 index = 0; index < count; index++) {
		indices[index] = (legacy_u8)index;
	}
	shape3d_hires_queue(0, type, count, indices, vertices, flags);
	bounds.left = 320;
	bounds.top = 200;
	bounds.right = 0;
	bounds.bottom = 0;
	shape3d_hires_update_bounds(0, type, &bounds);
}

static void queue(legacy_u8 type, legacy_u32 count, const struct SHAPE3D_HIRES_VECTOR *vertices)
{
	queue_flagged(type, count, vertices, 0);
}

/* Road dashes are attached polygons, not line primitives. Nearby markings
 * retain their weight; distant markings become finer without losing coverage. */
static void test_attached_polygon_weight(void)
{
	const legacy_f64 directions[][2] = {{1, 0}, {0, 1}, {0.8, 0.6}};
	const legacy_f64 depths[] = {600, 1000, 6400};
	legacy_u32 near_coverage[4][2];
	legacy_u32 distance_coverage[3] = {0};
	for (legacy_u32 direction = 0; direction < 3; direction++) {
		for (legacy_u32 distance = 0; distance < 3; distance++) {
			for (legacy_u32 translation = 0; translation < 4; translation++) {
				legacy_f64 depth = depths[distance];
				legacy_f64 shift = translation / 4.0;
				legacy_f64 tangent_x = directions[direction][0];
				legacy_f64 tangent_y = directions[direction][1];
				struct SHAPE3D_HIRES_VECTOR marking[4];
				for (legacy_u32 vertex = 0; vertex < 4; vertex++) {
					legacy_f64 along = (vertex < 2 ? -1 : 1) * depth / 16;
					legacy_f64 across = (vertex == 0 || vertex == 3 ? -1 : 1) * 1.5;
					marking[vertex].x =
						tangent_x * along - tangent_y * across + shift * depth / 640;
					marking[vertex].y =
						-(tangent_y * along + tangent_x * across + shift * depth / 640);
					marking[vertex].z = depth;
				}
				for (legacy_u32 winding = 0; winding < 2; winding++) {
					struct SHAPE3D_HIRES_VECTOR ordered[4];
					for (legacy_u32 vertex = 0; vertex < 4; vertex++) {
						ordered[vertex] = marking[winding ? 3 - vertex : vertex];
					}
					reset_target();
					queue_flagged(RENDER_PRIMITIVE_POLYGON, 4, ordered, 3);
					shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
					hires_end();
					const legacy_u8 *image = pixels();
					legacy_u32 coverage = count_color(8);
					distance_coverage[distance] += coverage;
					assert(coverage >= 80);
					assert(coverage <= 84 * (HIRES_SCALE + 1));
					if (distance == 0) {
						assert(coverage >= 80 * (HIRES_SCALE - 1));
						near_coverage[translation][winding] = coverage;
					} else if (distance == 2) {
						assert(coverage <= 84 * 2);
						assert(coverage < near_coverage[translation][winding]);
					}
					for (legacy_s32 along = -30; along <= 30; along++) {
						legacy_s32 x = (legacy_s32)(640 + tangent_x * along + shift);
						legacy_s32 y = (legacy_s32)(400 + tangent_y * along + shift);
						assert(image[y * HIRES_WIDTH + x] == 8);
					}
				}
			}
		}
	}
	assert(distance_coverage[0] > distance_coverage[1]);
	assert(distance_coverage[1] > distance_coverage[2]);
}

static void test_attached_polygon_midrange_weight(void)
{
	/* Keep the projected dash length and subpixel phase fixed. These distances
	 * cover the near cap, mid-range falloff, and distant visibility floor. */
	const legacy_f64 depths[] = {200, 250, 300, 400, 1000, 6400};
	const legacy_u32 expected_widths[] = {4, 3, 2, 1, 1, 1};
	for (legacy_u32 distance = 0; distance < sizeof(depths) / sizeof(depths[0]); distance++) {
		legacy_f64 depth = depths[distance];
		legacy_f64 shift = depth * 0.375 / 640;
		const struct SHAPE3D_HIRES_VECTOR marking[] = {{-depth / 16 + shift, -0.25 - shift, depth},
													   {depth / 16 + shift, -0.25 - shift, depth},
													   {depth / 16 + shift, 0.25 - shift, depth},
													   {-depth / 16 + shift, 0.25 - shift, depth}};
		reset_target();
		queue_flagged(RENDER_PRIMITIVE_POLYGON, 4, marking, 3);
		shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
		hires_end();
		const legacy_u8 *image = pixels();
		for (legacy_s32 x = 610; x <= 670; x++) {
			legacy_u32 coverage = 0;
			for (legacy_s32 y = 395; y <= 405; y++) {
				coverage += image[y * HIRES_WIDTH + x] == 8;
			}
			assert(coverage == expected_widths[distance]);
			assert(image[400 * HIRES_WIDTH + x] == 8);
		}
	}
}

static void test_attached_polygon_weight_follows_projection(void)
{
	static legacy_u8 reference[HIRES_WIDTH * HIRES_HEIGHT];
	/* A fractional translation distinguishes this medium-distance stroke
	 * from both the full near width and the minimum distant width. */
	const struct SHAPE3D_HIRES_VECTOR marking[] = {{-18.57421875, -0.42578125, 300},
												   {18.92578125, -0.42578125, 300},
												   {18.92578125, 0.07421875, 300},
												   {-18.57421875, 0.07421875, 300}};
	reset_target();
	queue_flagged(RENDER_PRIMITIVE_POLYGON, 4, marking, 3);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
	hires_end();
	assert(count_color(8) >= 160 && count_color(8) < 200);
	memcpy(reference, pixels(), sizeof(reference));

	/* Equal focal-length/depth ratios preserve both geometry and weight. */
	struct SHAPE3D_HIRES_VECTOR equivalent[4];
	for (legacy_u32 vertex = 0; vertex < 4; vertex++) {
		equivalent[vertex] = marking[vertex];
		equivalent[vertex].z *= 2;
	}
	projection_focal_length_x = projection_focal_length_y = 320;
	reset_target();
	queue_flagged(RENDER_PRIMITIVE_POLYGON, 4, equivalent, 3);
	/* Width must use the projection captured when the primitive was queued. */
	projection_focal_length_x = projection_focal_length_y = 160;
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
	hires_end();
	assert(memcmp(reference, pixels(), sizeof(reference)) == 0);

	/* Equivalent authored model sizes retain the same displayed weight. */
	const legacy_f64 scales[] = {20, 0.5};
	for (legacy_u32 model = 0; model < 2; model++) {
		legacy_f64 scale = scales[model];
		for (legacy_u32 vertex = 0; vertex < 4; vertex++) {
			equivalent[vertex].x = marking[vertex].x * scale;
			equivalent[vertex].y = marking[vertex].y * scale;
			equivalent[vertex].z = marking[vertex].z * scale;
		}
		reset_target();
		shape3d_hires_set_model_scale(scale);
		queue_flagged(RENDER_PRIMITIVE_POLYGON, 4, equivalent, 3);
		shape3d_hires_set_model_scale(1);
		shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
		hires_end();
		assert(memcmp(reference, pixels(), sizeof(reference)) == 0);
	}
}

static void test_attached_polygon_varies_along_depth(void)
{
	/* Perspective narrows the far end of a dash on a sloping road. */
	const struct SHAPE3D_HIRES_VECTOR marking[] = {
		{-100, -1.5, 1600}, {-100, 1.5, 1600}, {400, 1.5, 6400}, {400, -1.5, 6400}};
	reset_target();
	queue_flagged(RENDER_PRIMITIVE_POLYGON, 4, marking, 3);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
	hires_end();
	const legacy_u8 *image = pixels();
	assert(count_color(8) >= 80);
	for (legacy_s32 x = 605; x <= 675; x++) {
		legacy_u32 coverage = 0;
		for (legacy_s32 y = 397; y <= 403; y++) {
			coverage += image[y * HIRES_WIDTH + x] == 8;
		}
		assert(coverage >= 1);
		assert(image[400 * HIRES_WIDTH + x] == 8);
	}
}

static void test_polygon_weight_preserves_surfaces(void)
{
	static legacy_u8 original[HIRES_WIDTH * HIRES_HEIGHT];
	const struct SHAPE3D_HIRES_VECTOR panel[] = {
		{-50, -20, 200}, {50, -20, 200}, {50, 20, 200}, {-50, 20, 200}};
	for (legacy_u16 attached = 0; attached < 2; attached++) {
		reset_target();
		queue_flagged(RENDER_PRIMITIVE_POLYGON, 4, panel, attached ? 3 : 0);
		shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
		hires_end();
		assert(count_color(8) != 0);
		if (!attached) {
			memcpy(original, pixels(), sizeof(original));
		} else {
			assert(memcmp(original, pixels(), sizeof(original)) == 0);
		}
	}

	/* Unattached, subpixel road surfaces must not turn into thick strokes. */
	const struct SHAPE3D_HIRES_VECTOR thin[] = {
		{-400, -1.5, 6400}, {400, -1.5, 6400}, {400, 1.5, 6400}, {-400, 1.5, 6400}};
	reset_target();
	queue(RENDER_PRIMITIVE_POLYGON, 4, thin);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
	hires_end();
	assert(count_color(8) == 0);
}

static void test_attached_polygon_clipping(void)
{
	const struct SHAPE3D_HIRES_VECTOR marking[] = {
		{-7000, -1.5, 6400}, {7000, -1.5, 6400}, {7000, 1.5, 6400}, {-7000, 1.5, 6400}};
	reset_target();
	queue_flagged(RENDER_PRIMITIVE_POLYGON, 4, marking, 3);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
	hires_end();
	const legacy_u8 *image = pixels();
	assert(count_color(8) == HIRES_WIDTH * 2);
	for (legacy_s32 y = 399; y < 401; y++) {
		assert(image[y * HIRES_WIDTH] == 8);
		assert(image[y * HIRES_WIDTH + HIRES_WIDTH - 1] == 8);
	}

	target.sprite_raster_left = 150;
	target.sprite_raster_right = 170;
	target.sprite_top = 100;
	target.sprite_bottom = 110;
	reset_target();
	queue_flagged(RENDER_PRIMITIVE_POLYGON, 4, marking, 3);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
	hires_end();
	image = pixels();
	assert(count_color(8) == 80);
	for (legacy_s32 y = 0; y < HIRES_HEIGHT; y++) {
		for (legacy_s32 x = 0; x < HIRES_WIDTH; x++) {
			if (x < 600 || x >= 680 || y < 400 || y >= 440) {
				assert(image[y * HIRES_WIDTH + x] == 3);
			}
		}
	}
	target.sprite_raster_left = 0;
	target.sprite_raster_right = 320;
	target.sprite_top = 0;
	target.sprite_bottom = 200;
}

static void test_attached_polygon_occlusion(void)
{
	const struct SHAPE3D_HIRES_VECTOR parent[] = {
		{-5, -2.5, 80}, {5, -2.5, 80}, {5, 2.5, 80}, {-5, 2.5, 80}};
	/* Attached paint can be authored slightly behind its supporting surface. */
	const struct SHAPE3D_HIRES_VECTOR marking[] = {
		{-5, -0.025, 80.5}, {5, -0.025, 80.5}, {5, 0.025, 80.5}, {-5, 0.025, 80.5}};
	const struct SHAPE3D_HIRES_VECTOR nearer[] = {
		{-3.75, -1.25, 40}, {0, -1.25, 40}, {0, 1.25, 40}, {-3.75, 1.25, 40}};
	const legacy_u8 indices[] = {0, 1, 2, 3};
	for (legacy_u32 order = 0; order < 2; order++) {
		reset_target();
		queue(RENDER_PRIMITIVE_POLYGON, 4, parent);
		shape3d_hires_queue(1, RENDER_PRIMITIVE_POLYGON, 4, indices, marking, 3);
		shape3d_hires_update_bounds(1, RENDER_PRIMITIVE_POLYGON, &bounds);
		shape3d_hires_queue(2, RENDER_PRIMITIVE_POLYGON, 4, indices, nearer, 0);
		shape3d_hires_update_bounds(2, RENDER_PRIMITIVE_POLYGON, &bounds);
		if (order == 0) {
			shape3d_hires_render(2, RENDER_PRIMITIVE_POLYGON, 9, 0, 0, 0, 0);
		}
		shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
		shape3d_hires_render(1, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
		if (order != 0) {
			shape3d_hires_render(2, RENDER_PRIMITIVE_POLYGON, 9, 0, 0, 0, 0);
		}
		hires_end();
		const legacy_u8 *image = pixels();
		assert(count_color(8) != 0);
		for (legacy_s32 y = 398; y < 402; y++) {
			assert(image[y * HIRES_WIDTH + 639] == 9);
			assert(image[y * HIRES_WIDTH + 640] == 8);
			assert(image[y * HIRES_WIDTH + 660] == 8);
		}
	}
}

static void test_attached_polygon_preserves_interior_depth(void)
{
	/* This sloping dash projects to x 639.8..640.8. Its true inverse depth at
	 * pixel center 640.5 is 0.0093, behind the independent panel at 0.0095. */
	const struct SHAPE3D_HIRES_VECTOR marking[] = {{-0.2 / 6.4, -20 / 6.4, 100},
												   {0.8 / 5.76, -20 / 5.76, 1 / 0.009},
												   {0.8 / 5.76, 20 / 5.76, 1 / 0.009},
												   {-0.2 / 6.4, 20 / 6.4, 100}};
	const struct SHAPE3D_HIRES_VECTOR nearer[] = {{-2 / 6.08, -20 / 6.08, 1 / 0.0095},
												  {2 / 6.08, -20 / 6.08, 1 / 0.0095},
												  {2 / 6.08, 20 / 6.08, 1 / 0.0095},
												  {-2 / 6.08, 20 / 6.08, 1 / 0.0095}};
	const legacy_u8 indices[] = {0, 1, 2, 3};
	for (legacy_u32 order = 0; order < 2; order++) {
		reset_target();
		queue_flagged(RENDER_PRIMITIVE_POLYGON, 4, marking, 3);
		shape3d_hires_queue(1, RENDER_PRIMITIVE_POLYGON, 4, indices, nearer, 0);
		shape3d_hires_update_bounds(1, RENDER_PRIMITIVE_POLYGON, &bounds);
		if (order == 0) {
			shape3d_hires_render(1, RENDER_PRIMITIVE_POLYGON, 9, 0, 0, 0, 0);
		}
		shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
		if (order != 0) {
			shape3d_hires_render(1, RENDER_PRIMITIVE_POLYGON, 9, 0, 0, 0, 0);
		}
		hires_end();
		const legacy_u8 *image = pixels();
		assert(count_color(9) != 0);
		for (legacy_s32 y = 385; y < 415; y++) {
			assert(image[y * HIRES_WIDTH + 640] == 9);
		}
	}
}

static void test_projection_and_subpixel_edges(void)
{
	struct SHAPE3D_HIRES_VECTOR vector = {1, 1, 300};
	struct SHAPE3D_HIRES_POINT point;
	shape3d_hires_project(&vector, &point);
	assert(point.x > 642 && point.x < 643);
	assert(point.y > 397 && point.y < 398);
	const struct SHAPE3D_HIRES_VECTOR triangle[] = {
		{-100, -60, 300}, {100, -60, 300}, {-100, 60, 300}};
	reset_target();
	queue(RENDER_PRIMITIVE_POLYGON, 3, triangle);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
	hires_end();
	assert(count_color(7) > 40000);
	const legacy_u8 *image = pixels();
	legacy_u32 partial_blocks = 0;
	for (legacy_s32 y = 0; y < HIRES_HEIGHT; y += HIRES_SCALE) {
		for (legacy_s32 x = 0; x < HIRES_WIDTH; x += HIRES_SCALE) {
			legacy_u32 colored = 0;
			for (legacy_s32 row = 0; row < HIRES_SCALE; row++) {
				for (legacy_s32 column = 0; column < HIRES_SCALE; column++) {
					colored += image[(y + row) * HIRES_WIDTH + x + column] == 7;
				}
			}
			partial_blocks += colored != 0 && colored != HIRES_SCALE * HIRES_SCALE;
		}
	}
	assert(partial_blocks > 100);
	/* The detailed pass leaves the legacy image available for compatibility. */
	for (legacy_u32 index = 0; index < 320 * 200; index++) {
		assert(screen[index] == 3);
	}
}

static void test_near_plane_and_screen_clipping(void)
{
	const struct SHAPE3D_HIRES_VECTOR triangle[] = {{-10, -10, 1}, {50, -30, 100}, {0, 50, 100}};
	reset_target();
	queue(RENDER_PRIMITIVE_POLYGON, 3, triangle);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 9, 0, 0, 0, 0);
	hires_end();
	assert(count_color(9) > 100000);

	const struct SHAPE3D_HIRES_VECTOR line[] = {{-32768, 0, 12}, {32767, 0, 12}};
	reset_target();
	queue(RENDER_PRIMITIVE_LINE, 2, line);
	shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
	hires_end();
	assert(count_color(10) == HIRES_WIDTH * HIRES_SCALE);
	assert(pixels()[400 * HIRES_WIDTH] == 10);
	assert(pixels()[400 * HIRES_WIDTH + HIRES_WIDTH - 1] == 10);
}

static legacy_u32 colored_column(legacy_s32 x, legacy_u8 color)
{
	const legacy_u8 *image = pixels();
	legacy_u32 count = 0;
	for (legacy_s32 y = 0; y < HIRES_HEIGHT; y++) {
		count += image[y * HIRES_WIDTH + x] == color;
	}
	return count;
}

/* Nearby model lines keep their weight in every direction, while their
 * centerlines still move in high-resolution increments. */
static void test_line_weight(void)
{
	static legacy_u8 forward[HIRES_WIDTH * HIRES_HEIGHT];
	const struct SHAPE3D_HIRES_VECTOR lines[][2] = {{{-5, 0, 80}, {5, 0, 80}},
													{{0, -5, 80}, {0, 5, 80}},
													{{-5, -5, 80}, {5, 5, 80}},
													{{0, 0, 80}, {0, 0, 80}}};
	for (legacy_u32 direction = 0; direction < 4; direction++) {
		for (legacy_u32 reverse = 0; reverse < 2; reverse++) {
			const struct SHAPE3D_HIRES_VECTOR line[] = {lines[direction][reverse],
														lines[direction][1 - reverse]};
			reset_target();
			queue(RENDER_PRIMITIVE_LINE, 2, line);
			shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
			hires_end();
			const legacy_u8 *image = pixels();
			assert(image[400 * HIRES_WIDTH + 640] == 10);
			legacy_u32 coverage = count_color(10);
			if (direction < 2) {
				assert(coverage > 80 * 3 && coverage < 90 * HIRES_SCALE);
			} else if (direction == 2) {
				assert(coverage > 80 * HIRES_SCALE);
				assert(image[400 * HIRES_WIDTH + 645] == 3);
			} else {
				assert(coverage > 1 && coverage <= HIRES_SCALE * HIRES_SCALE);
			}
			if (reverse == 0) {
				memcpy(forward, image, sizeof(forward));
			} else {
				assert(memcmp(forward, image, sizeof(forward)) == 0);
			}
		}
	}

	/* Points and collapsed polygon edges retain their subpixel detail. */
	for (legacy_u32 type = 0; type < 2; type++) {
		legacy_u8 primitive = type == 0 ? RENDER_PRIMITIVE_POINT : RENDER_PRIMITIVE_POLYGON;
		reset_target();
		queue(primitive, type == 0 ? 1 : 2, lines[3]);
		shape3d_hires_render(0, primitive, 10, 0, 0, 0, 0);
		hires_end();
		assert(count_color(10) == 1);
	}
}

static void test_line_weight_follows_projection(void)
{
	static legacy_u8 close[HIRES_WIDTH * HIRES_HEIGHT];
	static legacy_u8 middle[HIRES_WIDTH * HIRES_HEIGHT];
	const legacy_f64 depths[] = {100, 200, 600, 6400};
	legacy_u32 widths[4];
	for (legacy_u32 distance = 0; distance < 4; distance++) {
		/* Keep the projected endpoints fixed while moving the line away. */
		legacy_f64 depth = depths[distance];
		const struct SHAPE3D_HIRES_VECTOR line[] = {{-depth / 16, 0, depth},
													{depth / 16, 0, depth}};
		reset_target();
		queue(RENDER_PRIMITIVE_LINE, 2, line);
		shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
		hires_end();
		assert(count_color(10) != 0);
		widths[distance] = colored_column(640, 10);
		if (distance == 0) {
			memcpy(close, pixels(), sizeof(close));
		} else if (distance == 1) {
			/* Close and medium-close lines retain the same full-weight stroke. */
			assert(memcmp(close, pixels(), sizeof(close)) == 0);
		} else if (distance == 2) {
			memcpy(middle, pixels(), sizeof(middle));
		}
	}
	assert(widths[0] == HIRES_SCALE);
	assert(widths[1] == HIRES_SCALE);
	assert(widths[1] > widths[2] && widths[2] > widths[3]);
	assert(widths[3] == 1);

	/* Equal focal-length/depth ratios must preserve both geometry and weight. */
	const struct SHAPE3D_HIRES_VECTOR equivalent[] = {{-37.5, 0, 1200}, {37.5, 0, 1200}};
	projection_focal_length_x = projection_focal_length_y = 320;
	reset_target();
	queue(RENDER_PRIMITIVE_LINE, 2, equivalent);
	/* Projection belongs to the queued shape, even if a later view changes it. */
	projection_focal_length_x = projection_focal_length_y = 160;
	shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
	hires_end();
	assert(memcmp(middle, pixels(), sizeof(middle)) == 0);

	/* Equivalent authored model sizes must not change their displayed weight. */
	const legacy_f64 scales[] = {20, 0.5};
	for (legacy_u32 model = 0; model < 2; model++) {
		legacy_f64 scale = scales[model];
		const struct SHAPE3D_HIRES_VECTOR line[] = {{-37.5 * scale, 0, 600 * scale},
													{37.5 * scale, 0, 600 * scale}};
		reset_target();
		shape3d_hires_set_model_scale(scale);
		queue(RENDER_PRIMITIVE_LINE, 2, line);
		shape3d_hires_set_model_scale(1);
		shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
		hires_end();
		assert(memcmp(middle, pixels(), sizeof(middle)) == 0);
	}
}

static void test_line_weight_varies_along_depth(void)
{
	const struct SHAPE3D_HIRES_VECTOR sloping[] = {{-29.296875, 0, 187.5}, {375, 0, 2400}};
	for (legacy_u32 reverse = 0; reverse < 2; reverse++) {
		const struct SHAPE3D_HIRES_VECTOR line[] = {sloping[reverse], sloping[1 - reverse]};
		reset_target();
		queue(RENDER_PRIMITIVE_LINE, 2, line);
		shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
		hires_end();
		assert(count_color(10) != 0);
		legacy_u32 near_width = colored_column(550, 10);
		legacy_u32 middle_width = colored_column(660, 10);
		legacy_u32 far_width = colored_column(730, 10);
		assert(near_width == HIRES_SCALE);
		assert(near_width > middle_width && middle_width > far_width);
		assert(far_width == 1);
	}
}

static void test_line_weight_after_clipping(void)
{
	const struct SHAPE3D_HIRES_VECTOR far_line[] = {{-6410, -400, 6400}, {-6410, 400, 6400}};
	reset_target();
	queue(RENDER_PRIMITIVE_LINE, 2, far_line);
	shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
	hires_end();
	/* A distant centerline outside the image must not inherit the near width. */
	assert(count_color(10) == 0);

	const struct SHAPE3D_HIRES_VECTOR crossing[] = {{-2, 0, 1}, {1000, 0, 6400}};
	for (legacy_u32 reverse = 0; reverse < 2; reverse++) {
		const struct SHAPE3D_HIRES_VECTOR line[] = {crossing[reverse], crossing[1 - reverse]};
		reset_target();
		queue(RENDER_PRIMITIVE_LINE, 2, line);
		shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
		hires_end();
		assert(count_color(10) != 0);
		assert(colored_column(640, 10) == HIRES_SCALE);
		assert(colored_column(740, 10) == 1);
	}
}

static void test_steep_line_stroke_has_no_holes(void)
{
	/* The rounded centerline can wander from the exact projected segment.
	 * This interior stroke pixel must survive in both endpoint orders. */
	const struct SHAPE3D_HIRES_VECTOR steep[] = {{0.4625, -3.525, 80}, {0.6875, 0.11875, 80}};
	for (legacy_u32 reverse = 0; reverse < 2; reverse++) {
		const struct SHAPE3D_HIRES_VECTOR line[] = {steep[reverse], steep[1 - reverse]};
		reset_target();
		queue(RENDER_PRIMITIVE_LINE, 2, line);
		shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
		hires_end();
		assert(count_color(10) != 0);
		assert(pixels()[416 * HIRES_WIDTH + 642] == 10);
	}
}

static void test_line_stroke_clipping(void)
{
	const struct SHAPE3D_HIRES_VECTOR edges[][2] = {{{-80.125, -5, 80}, {-80.125, 5, 80}},
													{{80, -5, 80}, {80, 5, 80}},
													{{-5, 50.125, 80}, {5, 50.125, 80}},
													{{-5, -50, 80}, {5, -50, 80}}};
	for (legacy_u32 edge = 0; edge < 4; edge++) {
		reset_target();
		queue(RENDER_PRIMITIVE_LINE, 2, edges[edge]);
		shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
		hires_end();
		/* The centerline is outside the image; its stroke still reaches in. */
		assert(count_color(10) > 0 && count_color(10) < 80 * HIRES_SCALE);
	}

	const struct SHAPE3D_HIRES_VECTOR clipped[][2] = {{{-5.125, -7.5, 80}, {-5.125, 7.5, 80}},
													  {{-7.5, 5.125, 80}, {7.5, 5.125, 80}}};
	target.sprite_raster_left = 150;
	target.sprite_raster_right = 170;
	target.sprite_top = 90;
	target.sprite_bottom = 110;
	for (legacy_u32 direction = 0; direction < 2; direction++) {
		reset_target();
		queue(RENDER_PRIMITIVE_LINE, 2, clipped[direction]);
		shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
		hires_end();
		assert(count_color(10) > 0 && count_color(10) < 20 * HIRES_SCALE * HIRES_SCALE);
		const legacy_u8 *image = pixels();
		for (legacy_s32 y = 0; y < HIRES_HEIGHT; y++) {
			for (legacy_s32 x = 0; x < HIRES_WIDTH; x++) {
				if (x < 600 || x >= 680 || y < 360 || y >= 440) {
					assert(image[y * HIRES_WIDTH + x] == 3);
				}
			}
		}
	}
	target.sprite_raster_left = 0;
	target.sprite_raster_right = 320;
	target.sprite_top = 0;
	target.sprite_bottom = 200;
}

static void test_line_stroke_occlusion(void)
{
	const struct SHAPE3D_HIRES_VECTOR panel[] = {
		{-3.75, -1.25, 40}, {0, -1.25, 40}, {0, 1.25, 40}, {-3.75, 1.25, 40}};
	const struct SHAPE3D_HIRES_VECTOR line[] = {{-2.5, 0, 80}, {2.5, 0, 80}};
	const legacy_u8 indices[] = {0, 1};
	for (legacy_u32 order = 0; order < 2; order++) {
		reset_target();
		queue(RENDER_PRIMITIVE_POLYGON, 4, panel);
		shape3d_hires_queue(1, RENDER_PRIMITIVE_LINE, 2, indices, line, 0);
		shape3d_hires_update_bounds(1, RENDER_PRIMITIVE_LINE, &bounds);
		if (order == 0) {
			shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
		}
		shape3d_hires_render(1, RENDER_PRIMITIVE_LINE, 8, 0, 0, 0, 0);
		if (order != 0) {
			shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
		}
		hires_end();
		for (legacy_s32 y = 398; y < 402; y++) {
			assert(pixels()[y * HIRES_WIDTH + 639] == 7);
			assert(pixels()[y * HIRES_WIDTH + 640] == 8);
		}
		assert(count_color(8) > 0);
		assert(colored_column(650, 8) == HIRES_SCALE);
	}
}

static void test_materials_and_rounded_primitives(void)
{
	const struct SHAPE3D_HIRES_VECTOR rectangle[] = {
		{-50, -50, 200}, {50, -50, 200}, {50, 50, 200}, {-50, 50, 200}};
	reset_target();
	queue(RENDER_PRIMITIVE_POLYGON, 4, rectangle);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 5, 6, 0, 2, 0xAA55);
	hires_end();
	assert(count_color(5) == 51200);
	assert(count_color(6) == 51200);
	assert(pixels()[240 * HIRES_WIDTH + 480] == 5);
	assert(pixels()[240 * HIRES_WIDTH + 481] == 6);

	reset_target();
	queue(RENDER_PRIMITIVE_POLYGON, 4, rectangle);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON | RENDER_PRIMITIVE_GHOST_FLAG, 5, 6, 0, 0, 0);
	hires_end();
	assert(count_color(0) == 51200);
	assert(count_color(5) == 0);

	const struct SHAPE3D_HIRES_VECTOR sphere[] = {{0, 0, 300}, {20, 0, 300}};
	reset_target();
	queue(RENDER_PRIMITIVE_SPHERE, 2, sphere);
	shape3d_hires_render(0, RENDER_PRIMITIVE_SPHERE, 11, 0, 0, 0, 0);
	hires_end();
	assert(count_color(11) > 1000);
	assert(pixels()[400 * HIRES_WIDTH + 640] == 11);
	assert(pixels()[380 * HIRES_WIDTH + 640] == 3);

	const struct SHAPE3D_HIRES_VECTOR wheel[] = {{-20, 0, 400}, {0, 0, 400},  {-20, 20, 400},
												 {0, 0, 420},	{20, 0, 420}, {0, 20, 420}};
	reset_target();
	queue(RENDER_PRIMITIVE_WHEEL, 6, wheel);
	shape3d_hires_render(0, RENDER_PRIMITIVE_WHEEL, 11, 12, 13, 0, 0);
	hires_end();
	assert(count_color(11) != 0);
	assert(count_color(12) != 0);
	assert(count_color(13) != 0);
}

static void test_disabled_and_reset(void)
{
	const struct SHAPE3D_HIRES_VECTOR point[] = {{0, 0, 100}};
	reset_target();
	queue(RENDER_PRIMITIVE_POINT, 1, point);
	shape3d_hires_reset();
	shape3d_hires_render(0, RENDER_PRIMITIVE_POINT, 17, 0, 0, 0, 0);
	hires_end();
	assert(count_color(17) == 0);
	hires_set_enabled(0);
	queue(RENDER_PRIMITIVE_POINT, 1, point);
	hires_set_enabled(1);
	assert(hires_begin(&target));
	shape3d_hires_render(0, RENDER_PRIMITIVE_POINT, 17, 0, 0, 0, 0);
	hires_end();
	assert(count_color(17) == 0);
}

/* Exercise the production transform and culling gates, which the direct raster
 * tests above intentionally bypass. Thin road and body panels must survive
 * until their fractional coordinates reach the high-resolution rasterizer. */
static legacy_u8 scene_vertices[10U * SHAPE3D_VERTEX_SIZE];
static legacy_u8 scene_primitives[64];
static legacy_u8 scene_visibility[16];
static legacy_u8 scene_front_facing[16];
static legacy_u8 scene_polyinfo[POLYINFO_SUPERSIGHT_DATA_SIZE];
static legacy_s16 scene_colors[] = {7, 8, 9, 10};
static legacy_s16 scene_patterns[4];
static struct SHAPE3D scene_shape;
static struct TRANSFORMEDSHAPE3D scene_instance;

static void prepare_scene(const struct VECTOR *vertices, legacy_u32 count,
						  const legacy_u8 *primitives, legacy_u32 primitive_size,
						  legacy_s32 high_resolution)
{
	hires_shutdown();
	memset(screen, 3, 320 * 200);
	hires_set_enabled(high_resolution);
	sprite_select_target(&target);
	memset(&scene_shape, 0, sizeof(scene_shape));
	memset(&scene_instance, 0, sizeof(scene_instance));
	memset(scene_primitives, 0, sizeof(scene_primitives));
	memset(scene_visibility, 255, sizeof(scene_visibility));
	memcpy(scene_primitives, primitives, primitive_size);
	scene_shape.shape3d_numverts = (legacy_u16)count;
	scene_shape.shape3d_vertex_bytes = scene_vertices;
	scene_shape.shape3d_numpaints = 1;
	scene_shape.shape3d_primitives = scene_primitives;
	scene_shape.shape3d_visibility_masks = scene_visibility;
	scene_shape.shape3d_front_facing_masks = scene_front_facing;
	for (legacy_u32 index = 0; index < count; index++) {
		shape3d_vertex_write(&scene_shape, (legacy_u16)index, &vertices[index]);
	}
	scene_instance.shapeptr = &scene_shape;
	scene_instance.rectptr = &bounds;
	scene_instance.ts_flags = 10; /* Pretransformed translation and bounding rectangle. */
	scene_instance.culling_distance = 1024;
	bounds.left = 320;
	bounds.top = 200;
	bounds.right = bounds.bottom = 0;
	polyinfoptr = scene_polyinfo;
	struct RECTANGLE clip = {0, 320, 0, 200};
	select_cliprect_rotate(0, 0, 0, &clip, 0);
	material_clrlist_ptr_cpy = scene_colors;
	material_clrlist2_ptr_cpy = scene_colors;
	material_patlist_ptr_cpy = scene_patterns;
	material_patlist2_ptr_cpy = scene_patterns;
}

static void test_visible_line_overhang_survives_culling(void)
{
	/* These centerlines project just outside the left and top viewport edges. */
	const struct VECTOR lines[][2] = {{{-121, -10, 120}, {-121, 10, 120}},
									  {{-10, 75, 119}, {10, 75, 119}}};
	const legacy_u8 primitive[] = {2, 0, 0, 0, 1, 0, 0};
	projection_focal_length_x = projection_focal_length_y = 159;
	for (legacy_u32 edge = 0; edge < 2; edge++) {
		prepare_scene(lines[edge], 2, primitive, sizeof(primitive), 1);
		assert(shape3d_transform_and_queue(&scene_instance) == 0);
		assert(polyinfonumpolys == 1);
		shape3d_render_queued_primitives();
		assert(count_color(7) != 0);
		assert(pixels()[edge == 0 ? 400 * HIRES_WIDTH : 640] == 7);
	}
	projection_focal_length_x = projection_focal_length_y = 160;
}

static void test_line_weight_in_half_scale_view(void)
{
	static legacy_u8 full_scale[HIRES_WIDTH * HIRES_HEIGHT];
	const struct VECTOR line[] = {{-100, 0, 1600}, {100, 0, 1600}};
	const legacy_u8 primitive[] = {2, 0, 0, 0, 1, 0, 0};
	for (legacy_s16 half_scale = 0; half_scale < 2; half_scale++) {
		prepare_scene(line, 2, primitive, sizeof(primitive), 1);
		struct RECTANGLE clip = {0, 320, 0, 200};
		select_cliprect_rotate(0, 0, 0, &clip, half_scale);
		assert(shape3d_transform_and_queue(&scene_instance) == 0);
		shape3d_render_queued_primitives();
		assert(count_color(7) != 0);
		if (half_scale == 0) {
			memcpy(full_scale, pixels(), sizeof(full_scale));
		} else {
			assert(memcmp(full_scale, pixels(), sizeof(full_scale)) == 0);
		}
	}
}

static void test_thin_polygon_and_attached_detail(void)
{
	const struct VECTOR road[] = {
		{-100, -10, 2000}, {-100, -10, 4000}, {100, -10, 4000}, {100, -10, 2000}};
	/* Rejecting the road must also discard its dependent line. Place the line
	 * along a side edge so its thicker stroke leaves some road visible. */
	const legacy_u8 primitives[] = {4, 0, 0, 0, 1, 2, 3, 2, 2, 1, 0, 1, 0, 0};
	prepare_scene(road, 4, primitives, sizeof(primitives), 0);
	assert(shape3d_transform_and_queue(&scene_instance) == LEGACY_U16_MAX);
	assert(polyinfonumpolys == 0);

	prepare_scene(road, 4, primitives, sizeof(primitives), 1);
	assert(shape3d_transform_and_queue(&scene_instance) == 0);
	assert(polyinfonumpolys == 2);
	shape3d_render_queued_primitives();
	assert(count_color(7) > 0);
	assert(count_color(8) > 0);
	assert(pixels()[402 * HIRES_WIDTH + 640] == 7);
}

static void test_full_polygon_winding(void)
{
	const struct VECTOR panel[] = {
		{-30, -20, 200}, {-30, 0, 200}, {-30, 20, 200}, {30, 20, 200}, {30, -20, 200}};
	const legacy_u8 forward[] = {5, 0, 0, 0, 1, 2, 3, 4, 0, 0};
	const legacy_u8 reverse[] = {5, 0, 0, 4, 3, 2, 1, 0, 0, 0};
	prepare_scene(panel, 5, forward, sizeof(forward), 0);
	assert(shape3d_transform_and_queue(&scene_instance) == LEGACY_U16_MAX);
	assert(polyinfonumpolys == 0);

	prepare_scene(panel, 5, forward, sizeof(forward), 1);
	assert(shape3d_transform_and_queue(&scene_instance) == 0);
	assert(polyinfonumpolys == 1);
	shape3d_render_queued_primitives();
	assert(count_color(7) > 20000);
	assert(pixels()[400 * HIRES_WIDTH + 640] == 7);

	prepare_scene(panel, 5, reverse, sizeof(reverse), 1);
	assert(shape3d_transform_and_queue(&scene_instance) == LEGACY_U16_MAX);
	assert(polyinfonumpolys == 0);
	shape3d_render_queued_primitives();
	assert(count_color(7) == 0);
}

static void test_clipped_polygon_visibility(void)
{
	const struct VECTOR triangle[] = {{-10, -10, 1}, {0, 50, 100}, {50, -30, 100}};
	const legacy_u8 forward[] = {3, 0, 0, 0, 1, 2, 0, 0};
	const legacy_u8 reverse[] = {3, 0, 0, 2, 1, 0, 0, 0};
	prepare_scene(triangle, 3, forward, sizeof(forward), 1);
	assert(shape3d_transform_and_queue(&scene_instance) == 0);
	assert(polyinfonumpolys == 1);
	shape3d_render_queued_primitives();
	assert(count_color(7) > 100000);

	prepare_scene(triangle, 3, reverse, sizeof(reverse), 1);
	assert(shape3d_transform_and_queue(&scene_instance) == LEGACY_U16_MAX);
	assert(polyinfonumpolys == 0);
}

static void test_wheel_face_and_sort_depth(void)
{
	const struct VECTOR wheel[] = {{0, -1, 400}, {-20, -1, 400}, {0, 0, 400},
								   {5, -1, 420}, {-15, -1, 420}, {5, 0, 420}};
	const legacy_u8 primitive[] = {12, 0, 0, 0, 1, 2, 3, 4, 5, 0, 0};
	prepare_scene(wheel, 6, primitive, sizeof(primitive), 0);
	assert(shape3d_transform_and_queue(&scene_instance) == 0);
	assert(polyinfonumpolys == 1);
	assert(LEGACY_READ_U16_LE(scene_polyinfo) == 420);

	prepare_scene(wheel, 6, primitive, sizeof(primitive), 1);
	assert(shape3d_transform_and_queue(&scene_instance) == 0);
	assert(polyinfonumpolys == 1);
	assert(LEGACY_READ_U16_LE(scene_polyinfo) == 400);
	shape3d_render_queued_primitives();
	assert(count_color(7) + count_color(8) + count_color(9) != 0);
}

static void test_crossing_surfaces_use_pixel_depth(void)
{
	const struct VECTOR panels[] = {{-60, -30, 100}, {-60, 30, 100}, {60, 30, 300}, {60, -30, 300},
									{-60, -30, 300}, {-60, 30, 300}, {60, 30, 100}, {60, -30, 100}};
	const legacy_u8 forward[] = {4, 0, 0, 0, 1, 2, 3, 4, 0, 1, 4, 5, 6, 7, 0, 0};
	const legacy_u8 reverse[] = {4, 0, 1, 4, 5, 6, 7, 4, 0, 0, 0, 1, 2, 3, 0, 0};
	for (legacy_u32 order = 0; order < 2; order++) {
		prepare_scene(panels, 8, order == 0 ? forward : reverse, sizeof(forward), 1);
		assert(shape3d_transform_and_queue(&scene_instance) == 0);
		assert(polyinfonumpolys == 2);
		shape3d_render_queued_primitives();
		/* The equal average depths cannot order these intersecting panels:
		 * each is nearer on a different side of the image. */
		assert(pixels()[400 * HIRES_WIDTH + 600] == 7);
		assert(pixels()[400 * HIRES_WIDTH + 680] == 8);
	}
}

/* Different track elements can overlap even when their tile centers have
 * the opposite painter order. Preserve depth across every shape boundary. */
static void queue_polygon_shape(legacy_u32 index, legacy_s32 depth_mode,
								const struct SHAPE3D_HIRES_VECTOR *vertices)
{
	const legacy_u8 indices[] = {0, 1, 2, 3};
	shape3d_hires_begin_shape(index, depth_mode);
	shape3d_hires_queue(index, RENDER_PRIMITIVE_POLYGON, 4, indices, vertices, 0);
}

static void test_separate_shapes_use_pixel_depth(void)
{
	const struct SHAPE3D_HIRES_VECTOR panels[][4] = {
		{{-60, -30, 100}, {-60, 30, 100}, {60, 30, 300}, {60, -30, 300}},
		{{-60, -30, 300}, {-60, 30, 300}, {60, 30, 100}, {60, -30, 100}}};
	const legacy_s32 modes[] = {SHAPE3D_HIRES_DEPTH_ORDERED, SHAPE3D_HIRES_DEPTH_SORTED};
	for (legacy_u32 first_mode = 0; first_mode < 2; first_mode++) {
		for (legacy_u32 second_mode = 0; second_mode < 2; second_mode++) {
			for (legacy_u32 order = 0; order < 2; order++) {
				reset_target();
				queue_polygon_shape(0, modes[first_mode], panels[0]);
				queue_polygon_shape(1, modes[second_mode], panels[1]);
				for (legacy_u32 pass = 0; pass < 2; pass++) {
					legacy_u32 index = pass ^ order;
					shape3d_hires_render(index, RENDER_PRIMITIVE_POLYGON, (legacy_u16)(7 + index),
										 0, 0, 0, 0);
				}
				hires_end();
				assert(pixels()[400 * HIRES_WIDTH + 600] == 7);
				assert(pixels()[400 * HIRES_WIDTH + 680] == 8);
			}
		}
	}
}

static void test_separate_shapes_preserve_authored_overlays(void)
{
	const struct SHAPE3D_HIRES_VECTOR parent[] = {
		{-5, -2.5, 80}, {5, -2.5, 80}, {5, 2.5, 80}, {-5, 2.5, 80}};
	const struct SHAPE3D_HIRES_VECTOR marking[] = {
		{-5, -0.25, 80.5}, {5, -0.25, 80.5}, {5, 0.25, 80.5}, {-5, 0.25, 80.5}};
	const struct SHAPE3D_HIRES_VECTOR middle[] = {
		{-5, -2.5, 80.25}, {5, -2.5, 80.25}, {5, 2.5, 80.25}, {-5, 2.5, 80.25}};
	const struct SHAPE3D_HIRES_VECTOR nearer[] = {
		{-2.5, -1.25, 40}, {0, -1.25, 40}, {0, 1.25, 40}, {-2.5, 1.25, 40}};
	const legacy_u8 indices[] = {0, 1, 2, 3};
	const legacy_s32 modes[] = {SHAPE3D_HIRES_DEPTH_ORDERED, SHAPE3D_HIRES_DEPTH_SORTED};
	for (legacy_u32 mode = 0; mode < 2; mode++) {
		for (legacy_u32 order = 0; order < 2; order++) {
			reset_target();
			queue_polygon_shape(0, modes[mode], parent);
			/* Unsorted roads retain authored paint order even without a
			 * primitive attachment flag; sorted models use explicit decals. */
			shape3d_hires_queue(1, RENDER_PRIMITIVE_POLYGON, 4, indices, marking,
								mode == 0 ? 0 : 2);
			queue_polygon_shape(2, SHAPE3D_HIRES_DEPTH_SORTED, middle);
			queue_polygon_shape(3, SHAPE3D_HIRES_DEPTH_SORTED, nearer);
			if (order == 0) {
				shape3d_hires_render(2, RENDER_PRIMITIVE_POLYGON, 9, 0, 0, 0, 0);
				shape3d_hires_render(3, RENDER_PRIMITIVE_POLYGON, 10, 0, 0, 0, 0);
			}
			shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
			shape3d_hires_render(1, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
			if (order != 0) {
				shape3d_hires_render(2, RENDER_PRIMITIVE_POLYGON, 9, 0, 0, 0, 0);
				shape3d_hires_render(3, RENDER_PRIMITIVE_POLYGON, 10, 0, 0, 0, 0);
			}
			hires_end();
			/* Paint behind its own support stays visible, yet retains the
			 * support's depth against a surface between the road and paint. */
			assert(pixels()[400 * HIRES_WIDTH + 660] == 8);
			assert(pixels()[390 * HIRES_WIDTH + 660] == 7);
			assert(pixels()[400 * HIRES_WIDTH + 620] == 10);
		}
	}
}

static void test_ordered_shapes_keep_nearest_support(void)
{
	const struct SHAPE3D_HIRES_VECTOR parent[] = {
		{-5, -2.5, 80}, {5, -2.5, 80}, {5, 2.5, 80}, {-5, 2.5, 80}};
	const struct SHAPE3D_HIRES_VECTOR nearer[] = {
		{-2.5, -1.25, 40}, {2.5, -1.25, 40}, {2.5, 1.25, 40}, {-2.5, 1.25, 40}};
	const struct SHAPE3D_HIRES_VECTOR marking[] = {
		{-5, -0.25, 80.5}, {5, -0.25, 80.5}, {5, 0.25, 80.5}, {-5, 0.25, 80.5}};
	const struct SHAPE3D_HIRES_VECTOR middle[] = {
		{-3.75, -1.875, 60}, {3.75, -1.875, 60}, {3.75, 1.875, 60}, {-3.75, 1.875, 60}};
	const legacy_u8 indices[] = {0, 1, 2, 3};
	for (legacy_u32 order = 0; order < 2; order++) {
		reset_target();
		queue_polygon_shape(0, SHAPE3D_HIRES_DEPTH_ORDERED, parent);
		shape3d_hires_queue(1, RENDER_PRIMITIVE_POLYGON, 4, indices, nearer, 0);
		shape3d_hires_queue(2, RENDER_PRIMITIVE_POLYGON, 4, indices, marking, 0);
		queue_polygon_shape(3, SHAPE3D_HIRES_DEPTH_SORTED, middle);
		if (order == 0) {
			shape3d_hires_render(3, RENDER_PRIMITIVE_POLYGON, 10, 0, 0, 0, 0);
		}
		shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
		shape3d_hires_render(1, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
		shape3d_hires_render(2, RENDER_PRIMITIVE_POLYGON, 9, 0, 0, 0, 0);
		if (order != 0) {
			shape3d_hires_render(3, RENDER_PRIMITIVE_POLYGON, 10, 0, 0, 0, 0);
		}
		hires_end();
		/* The nearer second surface advances the shape's depth. Subsequent
		 * rear-authored paint must not move it back behind the other shape. */
		assert(pixels()[400 * HIRES_WIDTH + 660] == 9);
		assert(pixels()[390 * HIRES_WIDTH + 660] == 8);
	}
}

static void test_scene_depth_resets_between_draws(void)
{
	const struct SHAPE3D_HIRES_VECTOR nearer[] = {
		{-5, -2.5, 80}, {5, -2.5, 80}, {5, 2.5, 80}, {-5, 2.5, 80}};
	const struct SHAPE3D_HIRES_VECTOR farther[] = {
		{-10, -5, 160}, {10, -5, 160}, {10, 5, 160}, {-10, 5, 160}};
	reset_target();
	queue_polygon_shape(0, SHAPE3D_HIRES_DEPTH_SORTED, nearer);
	queue_polygon_shape(1, SHAPE3D_HIRES_DEPTH_SORTED, farther);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
	hires_end();
	assert(pixels()[400 * HIRES_WIDTH + 640] == 7);
	assert(hires_begin(&target));
	shape3d_hires_render(1, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
	hires_end();
	assert(pixels()[400 * HIRES_WIDTH + 640] == 8);
	assert(hires_begin(&target));
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
	shape3d_hires_reset();
	queue_polygon_shape(0, SHAPE3D_HIRES_DEPTH_SORTED, farther);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
	hires_end();
	assert(pixels()[400 * HIRES_WIDTH + 640] == 8);
}

static void test_separate_shapes_preserve_ghost_holes(void)
{
	const struct SHAPE3D_HIRES_VECTOR ghost[] = {
		{-5, -2.5, 80}, {5, -2.5, 80}, {5, 2.5, 80}, {-5, 2.5, 80}};
	const struct SHAPE3D_HIRES_VECTOR farther[] = {
		{-10, -5, 160}, {10, -5, 160}, {10, 5, 160}, {-10, 5, 160}};
	for (legacy_u32 order = 0; order < 2; order++) {
		reset_target();
		queue_polygon_shape(0, SHAPE3D_HIRES_DEPTH_SORTED, ghost);
		queue_polygon_shape(1, SHAPE3D_HIRES_DEPTH_ORDERED, farther);
		if (order == 0) {
			shape3d_hires_render(1, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
		}
		shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON | RENDER_PRIMITIVE_GHOST_FLAG, 8, 0, 0, 0,
							 0);
		if (order != 0) {
			shape3d_hires_render(1, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
		}
		hires_end();
		const legacy_u8 *image = pixels();
		for (legacy_s32 y = 380; y < 420; y++) {
			for (legacy_s32 x = 600; x < 680; x++) {
				legacy_u32 bit = ((y & 1) == 0 ? 8U : 0U) + 7U - (x & 7);
				legacy_u8 expected = (PRERENDER_BLACK_GRILLE_PATTERN & (1U << bit)) != 0 ? 0 : 7;
				assert(image[y * HIRES_WIDTH + x] == expected);
			}
		}
	}
}

static void test_background_shapes_do_not_occlude_scene(void)
{
	const struct SHAPE3D_HIRES_VECTOR background[] = {
		{-5, -2.5, 80}, {5, -2.5, 80}, {5, 2.5, 80}, {-5, 2.5, 80}};
	const struct SHAPE3D_HIRES_VECTOR farther[] = {
		{-10, -5, 160}, {0, -5, 160}, {0, 5, 160}, {-10, 5, 160}};
	const legacy_s32 modes[] = {SHAPE3D_HIRES_DEPTH_ORDERED, SHAPE3D_HIRES_DEPTH_SORTED};
	for (legacy_u32 mode = 0; mode < 2; mode++) {
		reset_target();
		/* Clouds use an artificial distance and must not hide world geometry. */
		queue_polygon_shape(0, SHAPE3D_HIRES_DEPTH_BACKGROUND, background);
		queue_polygon_shape(1, modes[mode], farther);
		shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
		shape3d_hires_render(1, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
		hires_end();
		assert(pixels()[400 * HIRES_WIDTH + 620] == 8);
		assert(pixels()[400 * HIRES_WIDTH + 660] == 7);
	}
}

/* Opposite map corners are more than 32767 units apart after a diagonal
 * camera rotation, even though each world coordinate fits in a signed word. */
static void test_far_diagonal_geometry(void)
{
	const struct VECTOR panels[] = {
		{-2000, -1000, -1200}, {-2000, 1000, -1200}, {2000, 1000, 1200},  {2000, -1000, 1200},
		{-2000, -1000, 1200},  {-2000, 1000, 1200},	 {2000, 1000, -1200}, {2000, -1000, -1200}};
	const legacy_u8 forward[] = {4, 0, 0, 0, 1, 2, 3, 4, 0, 1, 4, 5, 6, 7, 0, 0};
	const legacy_u8 reverse[] = {4, 0, 1, 4, 5, 6, 7, 4, 0, 0, 0, 1, 2, 3, 0, 0};
	for (legacy_u32 order = 0; order < 2; order++) {
		prepare_scene(panels, 8, order == 0 ? forward : reverse, sizeof(forward), 1);
		struct RECTANGLE clip = {0, 320, 0, 200};
		select_cliprect_rotate(0, 0, -128, &clip, 0);
		scene_instance.ts_flags = 8;
		scene_instance.pos = (struct VECTOR){30000, 0, 30000};
		scene_instance.rotvec.z = 128;
		assert(shape3d_transform_and_queue(&scene_instance) == 0);
		assert(polyinfonumpolys == 2);
		shape3d_render_queued_primitives();
		assert(pixels()[400 * HIRES_WIDTH + 620] == 7);
		assert(pixels()[400 * HIRES_WIDTH + 660] == 8);
	}
}

static void test_far_primitive_sort_depth(void)
{
	const struct VECTOR panels[] = {
		{-2000, -1000, 1200},  {-2000, 1000, 1200},	 {2000, 1000, 1200},  {2000, -1000, 1200},
		{-2000, -1000, -1200}, {-2000, 1000, -1200}, {2000, 1000, -1200}, {2000, -1000, -1200}};
	const legacy_u8 forward[] = {4, 0, 0, 0, 1, 2, 3, 4, 0, 1, 4, 5, 6, 7, 0, 0};
	const legacy_u8 reverse[] = {4, 0, 1, 4, 5, 6, 7, 4, 0, 0, 0, 1, 2, 3, 0, 0};
	for (legacy_u32 order = 0; order < 2; order++) {
		prepare_scene(panels, 8, order == 0 ? forward : reverse, sizeof(forward), 1);
		struct RECTANGLE clip = {0, 320, 0, 200};
		select_cliprect_rotate(0, 0, -128, &clip, 0);
		scene_instance.ts_flags = 8;
		scene_instance.pos = (struct VECTOR){23000, 0, 23000};
		scene_instance.rotvec.z = 128;
		assert(shape3d_transform_and_queue(&scene_instance) == 0);
		assert(polyinfonumpolys == 2);
		/* Their depths straddle the signed 16-bit boundary. The far panel must
		 * precede the near panel regardless of the shape's authored order. */
		assert(polygon_next_index[order] == (legacy_s32)(1 - order));
		shape3d_render_queued_primitives();
		assert(pixels()[400 * HIRES_WIDTH + 640] == 8);
	}
}

static void test_body_panel_occludes_wheel(void)
{
	const struct VECTOR model[] = {
		{-100, -50, 500}, {-100, 50, 500}, {100, 50, 200}, {100, -50, 200}, {50, 0, 349},
		{30, 0, 349},	  {50, 20, 349},   {50, 0, 365},   {30, 0, 365},	{50, 20, 365}};
	const legacy_u8 primitives[] = {4, 0, 0, 0, 1, 2, 3, 12, 0, 1, 4, 5, 6, 7, 8, 9, 0, 0};
	prepare_scene(model, 10, primitives, sizeof(primitives), 1);
	assert(shape3d_transform_and_queue(&scene_instance) == 0);
	assert(polyinfonumpolys == 2);
	/* The body averages350 while the wheel is349. At the wheel's screen
	 * position the sloping body is actually nearer, at approximately288. */
	assert(LEGACY_READ_U16_LE(scene_polyinfo) == 350);
	assert(LEGACY_READ_U16_LE(scene_polyinfo + polygon_record_offsets[1]) == 349);
	shape3d_render_queued_primitives();
	assert(pixels()[400 * HIRES_WIDTH + 732] == 7);
	assert(count_color(8) + count_color(9) + count_color(10) == 0);
}

/* A road split into tiles must retain its continuous interior, even when the
 * tiles use different origins and model orientations. */
static void draw_joined_road(legacy_s32 split, legacy_s16 roll, legacy_s16 pitch, legacy_s16 yaw,
							 legacy_s16 depth, legacy_s16 half_scale, legacy_s16 reverse_order,
							 legacy_s16 turn)
{
	const struct VECTOR road[] = {
		{-200, 0, -1024}, {-200, 0, 1024}, {200, 0, 1024}, {200, 0, -1024}};
	const legacy_u8 primitive[] = {4, 1, 0, 0, 1, 2, 3, 0, 0};
	prepare_scene(road, 4, primitive, sizeof(primitive), 1);
	struct RECTANGLE clip = {0, 320, 0, 200};
	select_cliprect_rotate(roll, pitch, yaw, &clip, half_scale);
	scene_instance.ts_flags = 8;
	scene_instance.pos = (struct VECTOR){17, -233, depth};
	if (!split) {
		assert(shape3d_transform_and_queue(&scene_instance) == 0);
	} else {
		for (legacy_s16 index = 0; index < 2; index++) {
			legacy_s16 tile = reverse_order ? 1 - index : index;
			legacy_s16 rotation = tile ? turn : 0;
			struct MATRIX inverse;
			mat_rot_y(&inverse, -rotation);
			for (legacy_u16 vertex = 0; vertex < 4; vertex++) {
				struct VECTOR local = road[vertex];
				local.z /= 2;
				struct VECTOR rotated;
				mat_mul_vector(&local, &inverse, &rotated);
				shape3d_vertex_write(&scene_shape, vertex, &rotated);
			}
			scene_instance.rotvec.z = rotation;
			scene_instance.pos.z = depth + (tile ? 512 : -512) / (half_scale ? 2 : 1);
			assert(shape3d_transform_and_queue(&scene_instance) == 0);
		}
	}
	shape3d_render_queued_primitives();
	assert(count_color(7) > 1000);
}

static void test_joined_track_surfaces(void)
{
	static legacy_u8 continuous[HIRES_WIDTH * HIRES_HEIGHT];
	static legacy_u8 interior[HIRES_WIDTH * HIRES_HEIGHT];
	static legacy_u8 tiled[HIRES_WIDTH * HIRES_HEIGHT];
	static const legacy_s16 views[][4] = {
		{0, 0, 0, 1600}, {11, 17, 37, 1600}, {-19, -13, -71, 1600}, {7, 23, 9, 650}};
	for (legacy_u32 view = 0; view < sizeof(views) / sizeof(views[0]); view++) {
		for (legacy_s16 half_scale = 0; half_scale < 2; half_scale++) {
			draw_joined_road(0, views[view][0], views[view][1], views[view][2], views[view][3],
							 half_scale, 0, 0);
			memcpy(continuous, pixels(), sizeof(continuous));
			/* Splitting an outer edge can move an exact pixel-center tie by a
			 * rounding bit. Check the uniform interior on both sides of the
			 * silhouette; every tile join within the road must stay covered. */
			memset(interior, 0, sizeof(interior));
			for (legacy_s32 y = 1; y < HIRES_HEIGHT - 1; y++) {
				for (legacy_s32 x = 1; x < HIRES_WIDTH - 1; x++) {
					legacy_s32 offset = y * HIRES_WIDTH + x;
					legacy_u8 color = continuous[offset];
					legacy_s32 uniform = 1;
					for (legacy_s32 row = -1; row <= 1; row++) {
						for (legacy_s32 column = -1; column <= 1; column++) {
							uniform &= continuous[offset + row * HIRES_WIDTH + column] == color;
						}
					}
					interior[offset] = uniform ? color : 0;
				}
			}
			for (legacy_s16 turn = 0; turn < ANGLE_FULL_TURN; turn += ANGLE_QUARTER_TURN) {
				for (legacy_s16 order = 0; order < 2; order++) {
					draw_joined_road(1, views[view][0], views[view][1], views[view][2],
									 views[view][3], half_scale, order, turn);
					const legacy_u8 *image = pixels();
					if (turn == 0 && order == 0) {
						memcpy(tiled, image, sizeof(tiled));
					} else {
						assert(memcmp(tiled, image, sizeof(tiled)) == 0);
					}
					for (legacy_u32 pixel = 0; pixel < sizeof(interior); pixel++) {
						assert(interior[pixel] == 0 || image[pixel] == interior[pixel]);
					}
				}
			}
		}
	}
}

static void shared_edge_queue(const struct SHAPE3D_HIRES_VECTOR *vertices, const legacy_u8 *indices,
							  legacy_u16 index)
{
	shape3d_hires_queue(index, RENDER_PRIMITIVE_POLYGON, 3, indices, vertices, 0);
}

static void test_shared_edge_pixel_coverage(void)
{
	/* The diagonal projects through pixel centers; its two traversal
	 * directions previously rounded to different columns on some rows. */
	const struct SHAPE3D_HIRES_VECTOR rectangle[] = {
		{-455, 200, 640}, {315, 46, 640}, {-455, 46, 640}, {315, 200, 640}};
	const legacy_u8 forward[][3] = {{0, 1, 2}, {1, 0, 3}};
	const legacy_u8 reverse[][3] = {{2, 1, 0}, {3, 0, 1}};
	for (legacy_u32 winding = 0; winding < 2; winding++) {
		reset_target();
		shape3d_hires_begin_shape(0, 0);
		const legacy_u8(*indices)[3] = winding == 0 ? forward : reverse;
		shared_edge_queue(rectangle, indices[0], 0);
		shared_edge_queue(rectangle, indices[1], 1);
		shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
		shape3d_hires_render(1, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
		hires_end();
		const legacy_u8 *image = pixels();
		for (legacy_s32 y = 200; y < 354; y++) {
			for (legacy_s32 x = 185; x < 955; x++) {
				assert(image[y * HIRES_WIDTH + x] == 7);
			}
		}
	}
}

static void test_shared_edge_near_clipping(void)
{
	/* The clipped corner projects exactly to (113.5, 168.5). Reversing
	 * the near-plane crossing must preserve that covered pixel. */
	const struct SHAPE3D_HIRES_VECTOR triangle[] = {{-6, 0, 11}, {-4962, 5556, 1291}, {2, 0, 757}};
	const legacy_u8 indices[][3] = {{0, 1, 2}, {2, 1, 0}};
	for (legacy_u32 winding = 0; winding < 2; winding++) {
		reset_target();
		shape3d_hires_begin_shape(0, 0);
		shared_edge_queue(triangle, indices[winding], 0);
		shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
		hires_end();
		assert(pixels()[168 * HIRES_WIDTH + 113] == 7);
	}
}

/* A concave outline has four active edges below the notch. The left edge
 * ends and restarts exactly where the notch begins, and the final vertex
 * repeats the first. Pixel-center ties must survive clipping and batching. */
static void test_concave_polygon_scanlines(void)
{
	enum {
		CONCAVE_LEFT = 180,
		CONCAVE_RIGHT = 1060,
		CONCAVE_TOP = 63,
		CONCAVE_BOTTOM = 703,
		CONCAVE_NOTCH_LEFT = 420,
		CONCAVE_NOTCH_RIGHT = 820,
		CONCAVE_NOTCH_TOP = 383,
		CONCAVE_COLOR = 7,
		CONCAVE_BACKGROUND = 3,
		CONCAVE_CLIP_LEFT = 40,
		CONCAVE_CLIP_RIGHT = 240,
		CONCAVE_CLIP_TOP = 25,
		CONCAVE_CLIP_BOTTOM = 175,
		CONCAVE_VERTEX_COUNT = 10
	};
	const legacy_s32 outline[CONCAVE_VERTEX_COUNT][2] = {{CONCAVE_LEFT, CONCAVE_TOP},
														 {CONCAVE_RIGHT, CONCAVE_TOP},
														 {CONCAVE_RIGHT, CONCAVE_BOTTOM},
														 {CONCAVE_NOTCH_RIGHT, CONCAVE_BOTTOM},
														 {CONCAVE_NOTCH_RIGHT, CONCAVE_NOTCH_TOP},
														 {CONCAVE_NOTCH_LEFT, CONCAVE_NOTCH_TOP},
														 {CONCAVE_NOTCH_LEFT, CONCAVE_BOTTOM},
														 {CONCAVE_LEFT, CONCAVE_BOTTOM},
														 {CONCAVE_LEFT, CONCAVE_NOTCH_TOP},
														 {CONCAVE_LEFT, CONCAVE_TOP}};
	struct SHAPE3D_HIRES_VECTOR vertices[CONCAVE_VERTEX_COUNT];
	legacy_u8 indices[CONCAVE_VERTEX_COUNT];
	for (legacy_s32 collapsed = 0; collapsed < 2; collapsed++) {
		for (legacy_u32 index = 0; index < CONCAVE_VERTEX_COUNT; index++) {
			vertices[index].x =
				outline[index][0] + HIRES_SAMPLE_CENTER_OFFSET - projection_center_x * HIRES_SCALE;
			vertices[index].y =
				projection_center_y * HIRES_SCALE -
				((collapsed ? CONCAVE_TOP : outline[index][1]) + HIRES_SAMPLE_CENTER_OFFSET);
			vertices[index].z = projection_focal_length_x * HIRES_SCALE;
		}
		for (legacy_s32 winding = 0; winding < 2; winding++) {
			for (legacy_u32 index = 0; index < CONCAVE_VERTEX_COUNT; index++) {
				indices[index] = (legacy_u8)(winding ? CONCAVE_VERTEX_COUNT - 1 - index : index);
			}
			for (legacy_s32 clipped = 0; clipped < 2; clipped++) {
				target.sprite_raster_left = clipped ? CONCAVE_CLIP_LEFT : 0;
				target.sprite_raster_right =
					clipped ? CONCAVE_CLIP_RIGHT : HIRES_WIDTH / HIRES_SCALE;
				target.sprite_top = clipped ? CONCAVE_CLIP_TOP : 0;
				target.sprite_bottom = clipped ? CONCAVE_CLIP_BOTTOM : HIRES_HEIGHT / HIRES_SCALE;
				for (legacy_s32 batched = 0; batched < 2; batched++) {
					reset_target();
					shape3d_hires_queue(0, RENDER_PRIMITIVE_POLYGON, CONCAVE_VERTEX_COUNT, indices,
										vertices, 0);
					if (batched) {
						shape3d_hires_batch_begin();
					}
					shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, CONCAVE_COLOR, 0, 0, 0, 0);
					if (batched) {
						shape3d_hires_batch_end();
					}
					hires_end();
					const legacy_u8 *image = pixels();
					for (legacy_s32 y = 0; y < HIRES_HEIGHT; y++) {
						for (legacy_s32 x = 0; x < HIRES_WIDTH; x++) {
							legacy_s32 covered = !collapsed && x >= CONCAVE_LEFT &&
												 x < CONCAVE_RIGHT && y >= CONCAVE_TOP &&
												 y < CONCAVE_BOTTOM &&
												 (y < CONCAVE_NOTCH_TOP || x < CONCAVE_NOTCH_LEFT ||
												  x >= CONCAVE_NOTCH_RIGHT) &&
												 x >= target.sprite_raster_left * HIRES_SCALE &&
												 x < target.sprite_raster_right * HIRES_SCALE &&
												 y >= target.sprite_top * HIRES_SCALE &&
												 y < target.sprite_bottom * HIRES_SCALE;
							assert(image[y * HIRES_WIDTH + x] ==
								   (covered ? CONCAVE_COLOR : CONCAVE_BACKGROUND));
						}
					}
				}
			}
		}
	}
	target.sprite_raster_left = target.sprite_top = 0;
	target.sprite_raster_right = HIRES_WIDTH / HIRES_SCALE;
	target.sprite_bottom = HIRES_HEIGHT / HIRES_SCALE;
}

static void test_supersight_full_scene_queue(void)
{
	/* A dense scene exceeds both the old byte buffer and every 16-bit index.
	 * Three out-of-order points per shape also grow the queue mid-shape. */
	const struct VECTOR vertices[] = {{0, 0, 1000}, {0, 0, 2000}, {0, 0, 1500}};
	const legacy_u8 primitives[] = {1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 2, 0, 0};
	const legacy_u32 shape_count = 23334;
	const legacy_u32 count = shape_count * 3U;
	prepare_scene(vertices, 3, primitives, sizeof(primitives), 1);
	polyinfo_set_supersight(1);
	for (legacy_u32 index = 0; index < shape_count; index++) {
		scene_instance.pos.x = index == 0 ? -200 : index + 1U == shape_count ? 200 : 0;
		if (index + 1U == shape_count) {
			scene_primitives[2] = scene_primitives[6] = scene_primitives[10] = 1;
		}
		assert(shape3d_transform_and_queue(&scene_instance) == 0);
	}
	assert(polyinfonumpolys == count);
	assert(polyinfoptrnext == count * 10U);
	assert(polygon_record_offsets[count - 1U] > LEGACY_U16_MAX);
	polyinfo_link link = 1;
	for (legacy_u32 index = 0; index < count; index++) {
		legacy_u32 expected = index / 3U * 3U + (index % 3U + 1U) % 3U;
		assert(link == (polyinfo_link)expected);
		assert(polygon_record_offsets[link] == expected * 10U);
		assert(polyinfoptr[polygon_record_offsets[link] + 4U] == RENDER_PRIMITIVE_POINT);
		link = polygon_next_index[link];
	}
	assert(link == -1);
	shape3d_render_queued_primitives();
	assert(polyinfonumpolys == 0);
	assert(count_color(7) != 0);
	assert(count_color(8) != 0);

	/* Family 65536 must stay distinct from the zero/empty depth marker. */
	reset_target();
	const struct SHAPE3D_HIRES_VECTOR wide_point[] = {{100, 100, 1000}};
	const legacy_u8 wide_indices[] = {0};
	shape3d_hires_begin_shape(LEGACY_U16_MAX, 1);
	shape3d_hires_queue(LEGACY_U16_MAX, RENDER_PRIMITIVE_POINT, 1, wide_indices, wide_point, 0);
	bounds.left = 320;
	bounds.top = 200;
	bounds.right = bounds.bottom = 0;
	shape3d_hires_update_bounds(LEGACY_U16_MAX, RENDER_PRIMITIVE_POINT, &bounds);
	shape3d_hires_render(LEGACY_U16_MAX, RENDER_PRIMITIVE_POINT, 9, 0, 0, 0, 0);
	hires_end();
	assert(count_color(9) != 0);

	/* F12 restores the exact legacy limit even after native queues have grown.
	 * A new target buffer must also be safe when reusing the cached allocation. */
	polyinfo_set_supersight(0);
	const legacy_u8 point[] = {1, 0, 0, 0, 0, 0};
	prepare_scene(vertices, 1, point, sizeof(point), 0);
	for (legacy_u32 index = 1; index < POLYINFO_LEGACY_PRIMITIVE_CAPACITY; index++) {
		assert(shape3d_transform_and_queue(&scene_instance) == 0);
	}
	assert(shape3d_transform_and_queue(&scene_instance) == 1);
	assert(polyinfonumpolys == POLYINFO_LEGACY_PRIMITIVE_CAPACITY);
	prepare_scene(vertices, 1, point, sizeof(point), 1);
	polyinfo_set_supersight(1);
	for (legacy_u32 index = 0; index < 2000; index++) {
		assert(shape3d_transform_and_queue(&scene_instance) == 0);
	}
	assert(polyinfonumpolys == 2000);
	assert(polyinfoptrnext == 20000);
	shape3d_render_queued_primitives();
	assert(count_color(7) != 0);
	polyinfo_set_supersight(0);
}

/* Every primitive is queued before recording the immutable batch. The wide
 * surfaces provide enough work to exercise workers, while crossing depths,
 * authored overlays and patterned holes expose ordering or band-edge errors. */
static legacy_s32 draw_batch_scene(legacy_s32 batched, legacy_s32 ordered)
{
	static const struct SHAPE3D_HIRES_VECTOR vertices[][6] = {
		{{-2000, -1300, 2000}, {2000, -1300, 2000}, {2000, 1300, 2000}, {-2000, 1300, 2000}},
		{{-100, -60, 100}, {-100, 60, 100}, {100, 60, 300}, {100, -60, 300}},
		{{-100, -60, 300}, {-100, 60, 300}, {100, 60, 100}, {100, -60, 100}},
		{{-45, -40, 80}, {45, -40, 80}, {45, 40, 80}, {-45, 40, 80}},
		{{-43, 9.9, 80.5}, {43, 9.9, 80.5}, {43, 10.1, 80.5}, {-43, 10.1, 80.5}},
		{{10, -25, 60}, {40, -25, 60}, {40, 25, 60}, {10, 25, 60}},
		{{-20, -10, 40}, {-10, -10, 40}, {-20, 0, 40}, {-10, -10, 42}, {0, -10, 42}, {-10, 0, 42}},
		{{15, 10, 40}, {19, 10, 40}},
		{{-70, -45, 70}, {70, 45, 140}},
		{{-2, 1, 3}, {30, 20, 60}},
		{{-20, -0.1, 30}, {20, -0.1, 30}, {20, 0.1, 30}, {-20, 0.1, 30}},
		{{0.025, 0.016, 20}},
		/* These round inward from just beyond the left and top screen edges. */
		{{-100.0390625, 46.875, 100}},
		{{-84.375, 62.5390625, 100}, {-81.25, 62.5390625, 100}}};
	static const legacy_u8 types[] = {
		RENDER_PRIMITIVE_POLYGON, RENDER_PRIMITIVE_POLYGON,
		RENDER_PRIMITIVE_POLYGON, RENDER_PRIMITIVE_POLYGON,
		RENDER_PRIMITIVE_POLYGON, RENDER_PRIMITIVE_POLYGON | RENDER_PRIMITIVE_GHOST_FLAG,
		RENDER_PRIMITIVE_WHEEL,	  RENDER_PRIMITIVE_SPHERE,
		RENDER_PRIMITIVE_LINE,	  RENDER_PRIMITIVE_LINE,
		RENDER_PRIMITIVE_POLYGON, RENDER_PRIMITIVE_POINT,
		RENDER_PRIMITIVE_POINT,	  RENDER_PRIMITIVE_POLYGON};
	static const legacy_u8 counts[] = {4, 4, 4, 4, 4, 4, 6, 2, 2, 2, 4, 1, 1, 2};
	static const legacy_u16 paints[][5] = {
		{4, 0, 0, 0, 0},  {5, 6, 0, 2, 0xAA55}, {7, 0, 0, 0, 0},	{8, 0, 0, 0, 0},
		{9, 0, 0, 0, 0},  {0, 0, 0, 0, 0},		{10, 11, 12, 0, 0}, {13, 0, 0, 0, 0},
		{14, 0, 0, 0, 0}, {15, 0, 0, 0, 0},		{16, 0, 0, 0, 0},	{17, 0, 0, 0, 0},
		{18, 0, 0, 0, 0}, {19, 0, 0, 0, 0}};
	const legacy_u8 indices[] = {0, 1, 2, 3, 4, 5};
	shape3d_hires_reset();
	for (legacy_u32 index = 0; index < sizeof(counts); index++) {
		legacy_s32 mode = index == 0			  ? SHAPE3D_HIRES_DEPTH_BACKGROUND
						  : index == 3 && ordered ? SHAPE3D_HIRES_DEPTH_ORDERED
												  : SHAPE3D_HIRES_DEPTH_SORTED;
		if (index != 4) {
			shape3d_hires_begin_shape(index, mode);
		}
		legacy_u16 flags = index == 4 ? (ordered ? 0 : 2) : index == 10 ? 3 : 0;
		shape3d_hires_queue(index, types[index] & ~RENDER_PRIMITIVE_GHOST_FLAG, counts[index],
							indices, vertices[index], flags);
	}
	if (batched) {
		shape3d_hires_batch_begin();
	}
	for (legacy_u32 pass = 0; pass < sizeof(counts); pass++) {
		/* Reverse the intersecting surfaces without moving decals before
		 * their supporting geometry. Each order has its own serial oracle. */
		legacy_u32 index = !ordered && (pass == 1 || pass == 2) ? 3 - pass : pass;
		const legacy_u16 *paint = paints[index];
		shape3d_hires_render(index, types[index], paint[0], paint[1], paint[2], paint[3], paint[4]);
	}
	return batched ? shape3d_hires_batch_end() : 0;
}

static void test_parallel_batches_match_serial(void)
{
	static legacy_u8 reference[HIRES_WIDTH * HIRES_HEIGHT];
	const legacy_char *settings[] = {"0", "1", "2"};
	const legacy_char *original_setting = SDL_getenv("RESTUNTS_RENDER_WORKERS");
	legacy_char *saved_setting = original_setting != NULL ? SDL_strdup(original_setting) : NULL;
	assert(original_setting == NULL || saved_setting != NULL);
	projection_center_x = 160;
	projection_center_y = 100;
	projection_focal_length_x = projection_focal_length_y = 160;
	for (legacy_s32 clipped = 0; clipped < 2; clipped++) {
		target.sprite_raster_left = clipped ? 13 : 0;
		target.sprite_raster_right = clipped ? 307 : 320;
		target.sprite_top = clipped ? 7 : 0;
		target.sprite_bottom = clipped ? 193 : 200;
		bounds = (struct RECTANGLE){target.sprite_raster_left, target.sprite_raster_right,
									target.sprite_top, target.sprite_bottom};
		reset_target();
		assert(draw_batch_scene(0, !clipped) == 0);
		hires_end();
		/* Require coverage from the pattern, ghost and every primitive kind;
		 * otherwise an empty or fully occluded fixture could compare equal. */
		assert(count_color(0) != 0);
		for (legacy_u8 color = 4; color <= 17; color++) {
			assert(count_color(color) != 0);
		}
		if (!clipped) {
			assert(pixels()[100 * HIRES_WIDTH] == 18);
			assert(pixels()[100] == 19);
		}
		memcpy(reference, pixels(), sizeof(reference));
		for (legacy_s32 workers = 0; workers < 3; workers++) {
			assert(SDL_SetEnvironmentVariable(SDL_GetEnvironment(), "RESTUNTS_RENDER_WORKERS",
											  settings[workers], true));
			reset_target();
			for (legacy_s32 iteration = 0; iteration < 3; iteration++) {
				if (iteration != 0) {
					if (iteration == 2) {
						/* F12 releases surfaces and workers, then both must be
						 * ready again when SuperSight is enabled. */
						hires_set_enabled(0);
						hires_set_enabled(1);
					}
					assert(hires_begin(&target));
				}
#if defined(__DJGPP__)
				assert(draw_batch_scene(1, !clipped) == 0);
#else
				assert(draw_batch_scene(1, !clipped) == workers);
#endif
				hires_end();
				assert(memcmp(reference, pixels(), sizeof(reference)) == 0);
			}
		}
	}
	target.sprite_raster_left = target.sprite_top = 0;
	target.sprite_raster_right = 320;
	target.sprite_bottom = 200;
	reset_target();
	shape3d_hires_batch_begin();
	assert(shape3d_hires_batch_end() == 0);
	const struct SHAPE3D_HIRES_VECTOR point[] = {{0, 0, 100}};
	queue(RENDER_PRIMITIVE_POINT, 1, point);
	shape3d_hires_batch_begin();
	shape3d_hires_render(0, RENDER_PRIMITIVE_POINT, 17, 0, 0, 0, 0);
	/* Tiny previews should bypass worker synchronization even when enabled. */
	assert(shape3d_hires_batch_end() == 0);
	hires_end();
	assert(count_color(17) != 0);
	hires_shutdown();
	if (saved_setting != NULL) {
		assert(SDL_SetEnvironmentVariable(SDL_GetEnvironment(), "RESTUNTS_RENDER_WORKERS",
										  saved_setting, true));
		SDL_free(saved_setting);
	} else {
		assert(SDL_UnsetEnvironmentVariable(SDL_GetEnvironment(), "RESTUNTS_RENDER_WORKERS"));
	}
}

struct SHADOW_TEST_IMAGE {
	const legacy_u32 *pixels;
	legacy_f64 centroid_x, centroid_z;
	legacy_f64 half_width, half_length;
};
static struct SHADOW_TEST_IMAGE shadow_image;

/* A camera directly above the car makes the world footprint measurable
 * without depending on the shadow implementation's projection helpers. */
static void begin_shadow_scene(legacy_s16 car_height, legacy_s16 heading)
{
	const struct VECTOR camera = {0, 400, 0};
	const struct VECTOR car = {0, (legacy_s16)(car_height - camera.y), 0};
	reset_target();
	memset(&mat_temp, 0, sizeof(mat_temp));
	mat_temp.m._11 = 16384;
	mat_temp.m._23 = 16384;
	mat_temp.m._32 = -16384;
	projection_center_x = 160;
	projection_center_y = 100;
	projection_focal_length_x = projection_focal_length_y = 160;
	hires_depth_begin(0, HIRES_WIDTH, 0, HIRES_HEIGHT);
	shape3d_hires_shadows_begin(&camera);
	shape3d_hires_shadow_car(&car, heading, 40, 75);
	shadow_image.half_width = 40;
	shadow_image.half_length = 75;
}

static void queue_shadow_receiver(legacy_u32 index, legacy_s16 height, legacy_s32 receiver,
								  legacy_f64 right)
{
	const legacy_u8 indices[] = {0, 1, 2, 3};
	const struct SHAPE3D_HIRES_VECTOR plane[] = {{-150, -150, 400 - height},
												 {right, -150, 400 - height},
												 {right, 150, 400 - height},
												 {-150, 150, 400 - height}};
	shape3d_hires_begin_shape(index, SHAPE3D_HIRES_DEPTH_SORTED);
	shape3d_hires_set_shadow_receiver(receiver);
	shape3d_hires_queue(index, RENDER_PRIMITIVE_POLYGON, 4, indices, plane, 0);
}

static legacy_u32 finish_shadow_scene(legacy_s16 receiver_height, legacy_s16 heading,
									  legacy_u8 excluded_color)
{
	static legacy_u8 indexed[HIRES_WIDTH * HIRES_HEIGHT];
	legacy_u32 palette[256];
	for (legacy_u32 index = 0; index < 256; index++) {
		palette[index] = 0xFFC0C0C0U;
	}
	palette[15] = 0xFFFFFFFFU;
	memcpy(indexed, pixels(), sizeof(indexed));
	shape3d_hires_draw_shadows();
	hires_end();
	assert(memcmp(indexed, pixels(), sizeof(indexed)) == 0);
	const legacy_u32 *image = hires_framebuffer_argb(screen, palette);
	legacy_u32 changed = 0;
	shadow_image.pixels = image;
	shadow_image.centroid_x = shadow_image.centroid_z = 0;
	legacy_f64 weight_total = 0;
	for (legacy_s32 y = 0; image != NULL && y < HIRES_HEIGHT; y++) {
		for (legacy_s32 x = 0; x < HIRES_WIDTH; x++) {
			legacy_u32 offset = y * HIRES_WIDTH + x;
			if (image[offset] == palette[indexed[offset]]) {
				continue;
			}
			assert(indexed[offset] != excluded_color);
			assert((image[offset] & 0xFFFFFFU) < (palette[indexed[offset]] & 0xFFFFFFU));
			/* A short extension is permitted only toward world north (+Z).
			 * The soft fringe still stays within the other three bounds. */
			legacy_f64 world_x = (x + 0.5 - 640) * (400 - receiver_height) / 640;
			legacy_f64 world_z = (400 - y - 0.5) * (400 - receiver_height) / 640;
			legacy_f64 half_x =
				(heading & 256) == 0 ? shadow_image.half_width : shadow_image.half_length;
			legacy_f64 half_z =
				(heading & 256) == 0 ? shadow_image.half_length : shadow_image.half_width;
			legacy_f64 north_extension = (half_x < half_z ? half_x : half_z) * 0.7;
			assert(world_x >= -half_x - 0.5 && world_x <= half_x + 0.5);
			assert(world_z >= -half_z - 0.5 && world_z <= half_z + north_extension + 0.5);
			legacy_f64 weight = (palette[indexed[offset]] & 255U) - (image[offset] & 255U);
			shadow_image.centroid_x += world_x * weight;
			shadow_image.centroid_z += world_z * weight;
			weight_total += weight;
			changed++;
		}
	}
	if (weight_total > 0) {
		shadow_image.centroid_x /= weight_total;
		shadow_image.centroid_z /= weight_total;
	}
	return changed;
}

static void test_car_shadow_stays_below_and_shrinks(void)
{
	const legacy_s16 heights[] = {0, 48, 128};
	for (legacy_s16 heading = 0; heading <= 256; heading += 256) {
		legacy_u32 previous = HIRES_WIDTH * HIRES_HEIGHT;
		for (legacy_u32 height = 0; height < sizeof(heights) / sizeof(heights[0]); height++) {
			begin_shadow_scene(heights[height], heading);
			queue_shadow_receiver(0, 0, 1, 150);
			shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
			legacy_u32 coverage = finish_shadow_scene(0, heading, 0);
			assert(coverage > 100);
			assert(coverage < previous);
			previous = coverage;
		}
	}
}

/* A narrow open-wheel body, four cylinders and four suspension lines leave
 * deliberate gaps that a generic rounded rectangle cannot reproduce. */
static struct SHAPE3D shadow_model;
static legacy_u8 shadow_model_vertices[36 * SHAPE3D_VERTEX_SIZE];
static legacy_u8 shadow_model_primitives[7 + 4 * 9 + 4 * 5 + 2];

static void prepare_shadow_model(legacy_s16 scale)
{
	struct VECTOR vertices[36] = {{-12, 24, -60}, {12, 24, -60}, {12, 24, 60}, {-12, 24, 60}};
	memset(&shadow_model, 0, sizeof(shadow_model));
	shadow_model.shape3d_numverts = 36;
	shadow_model.shape3d_numprimitives = 9;
	shadow_model.shape3d_numpaints = 1;
	shadow_model.shape3d_vertex_bytes = shadow_model_vertices;
	shadow_model.shape3d_primitives = shadow_model_primitives;
	const legacy_u8 body[] = {4, 0, 0, 0, 1, 2, 3};
	memcpy(shadow_model_primitives, body, sizeof(body));
	legacy_u32 offset = sizeof(body);
	for (legacy_u32 wheel = 0; wheel < 4; wheel++) {
		legacy_s16 side = (wheel & 1) != 0 ? 1 : -1;
		legacy_s16 z = wheel < 2 ? -43 : 43;
		legacy_u8 base = (legacy_u8)(4 + wheel * 6);
		vertices[base] = (struct VECTOR){(legacy_s16)(side * 28), 12, z};
		vertices[base + 1] = (struct VECTOR){(legacy_s16)(side * 28), 12, (legacy_s16)(z + 12)};
		vertices[base + 2] = (struct VECTOR){(legacy_s16)(side * 28), 24, z};
		vertices[base + 3] = (struct VECTOR){(legacy_s16)(side * 40), 12, z};
		vertices[base + 4] = (struct VECTOR){(legacy_s16)(side * 40), 12, (legacy_s16)(z + 12)};
		vertices[base + 5] = (struct VECTOR){(legacy_s16)(side * 40), 24, z};
		shadow_model_primitives[offset++] = 12;
		shadow_model_primitives[offset++] = 0;
		shadow_model_primitives[offset++] = 0;
		for (legacy_u32 vertex = 0; vertex < 6; vertex++) {
			shadow_model_primitives[offset++] = (legacy_u8)(base + vertex);
		}
		base = (legacy_u8)(28 + wheel * 2);
		vertices[base] =
			(struct VECTOR){(legacy_s16)(side * 10), 12, (legacy_s16)(z < 0 ? -37 : 37)};
		vertices[base + 1] = (struct VECTOR){(legacy_s16)(side * 28), 12, z};
	}
	for (legacy_u32 wheel = 0; wheel < 4; wheel++) {
		shadow_model_primitives[offset++] = 2;
		shadow_model_primitives[offset++] = 0;
		shadow_model_primitives[offset++] = 0;
		shadow_model_primitives[offset++] = (legacy_u8)(28 + wheel * 2);
		shadow_model_primitives[offset++] = (legacy_u8)(29 + wheel * 2);
	}
	shadow_model_primitives[offset++] = 0;
	shadow_model_primitives[offset] = 0;
	for (legacy_u16 vertex = 0; vertex < 36; vertex++) {
		vertices[vertex].x *= scale;
		vertices[vertex].y *= scale;
		vertices[vertex].z *= scale;
		shape3d_vertex_write(&shadow_model, vertex, &vertices[vertex]);
	}
}

static legacy_u32 render_shadow_model(legacy_s16 heading, legacy_s16 scale)
{
	begin_shadow_scene(0, heading);
	shape3d_hires_shadow_model(&shadow_model);
	shadow_image.half_width = 40 * scale;
	shadow_image.half_length = 60 * scale;
	queue_shadow_receiver(0, 0, 1, 150);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
	return finish_shadow_scene(0, heading, 0);
}

static legacy_u32 shadow_samples_near(legacy_f64 world_x, legacy_f64 world_z)
{
	legacy_s32 center_x = (legacy_s32)(640 + world_x * 1.6);
	legacy_s32 center_y = (legacy_s32)(400 - world_z * 1.6);
	legacy_u32 changed = 0;
	for (legacy_s32 y = center_y - 1; y <= center_y + 1; y++) {
		for (legacy_s32 x = center_x - 1; x <= center_x + 1; x++) {
			changed += shadow_image.pixels[y * HIRES_WIDTH + x] != 0xFFC0C0C0U;
		}
	}
	return changed;
}

static void test_car_shadow_model_details_and_north_light(void)
{
	prepare_shadow_model(1);
	shape3d_hires_shadow_models_reset();
	for (legacy_s16 heading = 0; heading < 1024; heading += 256) {
		assert(render_shadow_model(heading, 1) > 100);
		/* Car heading changes the silhouette, never the sun's direction. */
		assert(shadow_image.centroid_x > -0.5 && shadow_image.centroid_x < 0.5);
		assert(shadow_image.centroid_z > 3 && shadow_image.centroid_z < 20);
	}
	assert(render_shadow_model(0, 1) > 100);
	legacy_f64 north = shadow_image.centroid_z;
	assert(shadow_samples_near(0, north) == 9);
	for (legacy_s16 side = -1; side <= 1; side += 2) {
		for (legacy_s16 end = -1; end <= 1; end += 2) {
			assert(shadow_samples_near(side * 34, end * 43 + north) == 9);
			assert(shadow_samples_near(side * 20, end * 40 + north) > 0);
			assert(shadow_samples_near(side * 20, end * 20 + north) == 0);
		}
	}
}

static void test_car_shadow_model_size_and_cache_reset(void)
{
	prepare_shadow_model(1);
	shape3d_hires_shadow_models_reset();
	legacy_u32 small = render_shadow_model(0, 1);
	/* Reloading a different car may reuse both the shape and vertex addresses.
	 * Its geometry must supersede the cached mask after resource invalidation. */
	prepare_shadow_model(2);
	shape3d_hires_shadow_models_reset();
	legacy_u32 large = render_shadow_model(0, 2);
	assert(large > small * 3.6 && large < small * 4.4);
	assert(shadow_samples_near(68, 86 + shadow_image.centroid_z) == 9);
	shape3d_hires_shadow_models_reset();
}

static void test_car_shadow_receiver_height(void)
{
	/* Elevated track under a car receives a shadow; a roof above it does
	 * not. Nor should a car high in the air darken remote terrain. */
	const legacy_s16 car_heights[] = {96, 24, 300};
	const legacy_s16 surface_heights[] = {64, 64, 0};
	for (legacy_u32 scene = 0; scene < 3; scene++) {
		begin_shadow_scene(car_heights[scene], 0);
		queue_shadow_receiver(0, surface_heights[scene], 1, 150);
		shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
		legacy_u32 coverage = finish_shadow_scene(surface_heights[scene], 0, 0);
		assert(scene == 0 ? coverage > 100 : coverage == 0);
	}
}

static void test_car_shadow_excludes_car_geometry(void)
{
	begin_shadow_scene(24, 0);
	/* An excluded car panel covers the left half of the footprint, even
	 * though it lies below the caster. The next shape is ordinary road. */
	queue_shadow_receiver(0, 8, 0, 0);
	const struct SHAPE3D_HIRES_VECTOR road[] = {
		{-150, -150, 400}, {150, -150, 400}, {150, 150, 400}, {-150, 150, 400}};
	queue_polygon_shape(1, SHAPE3D_HIRES_DEPTH_SORTED, road);
	shape3d_hires_render(1, RENDER_PRIMITIVE_POLYGON, 8, 0, 0, 0, 0);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 9, 0, 0, 0, 0);
	assert(pixels()[400 * HIRES_WIDTH + 620] == 9);
	assert(pixels()[400 * HIRES_WIDTH + 660] == 8);
	assert(finish_shadow_scene(0, 0, 9) > 100);
}

static void test_car_shadow_ground_fallback_and_reset(void)
{
	/* The ordinary flat ground has no queued depth surface. */
	begin_shadow_scene(24, 0);
	assert(finish_shadow_scene(0, 0, 0) > 100);
	begin_shadow_scene(24, 0);
	shape3d_hires_reset();
	assert(finish_shadow_scene(0, 0, 0) == 0);

	begin_shadow_scene(24, 0);
	hires_set_enabled(0);
	shape3d_hires_draw_shadows();
	hires_end();
	for (legacy_u32 index = 0; index < 320 * 200; index++) {
		assert(screen[index] == 3);
	}
	legacy_u32 palette[256] = {0};
	assert(hires_framebuffer_argb(screen, palette) == NULL);
	hires_set_enabled(1);
}

legacy_int main(void)
{
	screen = dos_memory_make_pointer(0xA000, 0);
	target.sprite_bitmapptr = (struct SHAPE2D *)screen;
	target.sprite_lineofs = rows;
	target.sprite_right = 320;
	target.sprite_bottom = 200;
	target.sprite_pitch = 320;
	target.sprite_buffer_width = 320;
	target.sprite_raster_right = 320;
	for (legacy_u32 row = 0; row < 200; row++) {
		LEGACY_WRITE_U16_LE(rows + row * 2, row * 320);
	}
	projection_center_x = 160;
	projection_center_y = 100;
	projection_focal_length_x = 160;
	projection_focal_length_y = 160;
	test_projection_and_subpixel_edges();
	test_attached_polygon_weight();
	test_attached_polygon_midrange_weight();
	test_attached_polygon_weight_follows_projection();
	test_attached_polygon_varies_along_depth();
	test_polygon_weight_preserves_surfaces();
	test_attached_polygon_clipping();
	test_attached_polygon_occlusion();
	test_attached_polygon_preserves_interior_depth();
	test_line_weight_follows_projection();
	test_line_weight_varies_along_depth();
	test_line_weight_after_clipping();
	test_line_weight();
	test_near_plane_and_screen_clipping();
	test_steep_line_stroke_has_no_holes();
	test_line_stroke_clipping();
	test_line_stroke_occlusion();
	test_materials_and_rounded_primitives();
	test_disabled_and_reset();
	test_visible_line_overhang_survives_culling();
	test_line_weight_in_half_scale_view();
	test_thin_polygon_and_attached_detail();
	test_full_polygon_winding();
	test_clipped_polygon_visibility();
	test_wheel_face_and_sort_depth();
	test_crossing_surfaces_use_pixel_depth();
	test_separate_shapes_use_pixel_depth();
	test_separate_shapes_preserve_authored_overlays();
	test_ordered_shapes_keep_nearest_support();
	test_scene_depth_resets_between_draws();
	test_separate_shapes_preserve_ghost_holes();
	test_background_shapes_do_not_occlude_scene();
	test_far_diagonal_geometry();
	test_far_primitive_sort_depth();
	test_supersight_full_scene_queue();
	test_body_panel_occludes_wheel();
	test_joined_track_surfaces();
	test_shared_edge_pixel_coverage();
	test_shared_edge_near_clipping();
	test_concave_polygon_scanlines();
	test_parallel_batches_match_serial();
	test_car_shadow_stays_below_and_shrinks();
	test_car_shadow_model_details_and_north_light();
	test_car_shadow_model_size_and_cache_reset();
	test_car_shadow_receiver_height();
	test_car_shadow_excludes_car_geometry();
	test_car_shadow_ground_fallback_and_reset();
	hires_shutdown();
	puts("High-resolution 3D projection and raster tests passed.");
	return 0;
}
