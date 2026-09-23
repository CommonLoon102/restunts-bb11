#include <assert.h>
#include <stdio.h>
#include <string.h>
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

static void queue(legacy_u8 type, legacy_u32 count, const struct SHAPE3D_HIRES_VECTOR *vertices)
{
	legacy_u8 indices[10];
	assert(count <= sizeof(indices));
	for (legacy_u32 index = 0; index < count; index++) {
		indices[index] = (legacy_u8)index;
	}
	shape3d_hires_queue(0, type, count, indices, vertices, 0);
	bounds.left = 320;
	bounds.top = 200;
	bounds.right = 0;
	bounds.bottom = 0;
	shape3d_hires_update_bounds(0, type, &bounds);
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

/* A model line keeps the weight of one legacy pixel in every direction,
 * while its centerline still moves in high-resolution increments. */
static void test_line_weight(void)
{
	static legacy_u8 forward[HIRES_WIDTH * HIRES_HEIGHT];
	const struct SHAPE3D_HIRES_VECTOR lines[][2] = {{{-40, 0, 640}, {40, 0, 640}},
													{{0, -40, 640}, {0, 40, 640}},
													{{-40, -40, 640}, {40, 40, 640}},
													{{0, 0, 640}, {0, 0, 640}}};
	for (legacy_u32 direction = 0; direction < 4; direction++) {
		for (legacy_u32 reverse = 0; reverse < 2; reverse++) {
			const struct SHAPE3D_HIRES_VECTOR line[] = {lines[direction][reverse],
														lines[direction][1 - reverse]};
			reset_target();
			queue(RENDER_PRIMITIVE_LINE, 2, line);
			shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
			hires_end();
			const legacy_u8 *image = pixels();
			for (legacy_s32 y = 399; y < 403; y++) {
				for (legacy_s32 x = 639; x < 643; x++) {
					assert(image[y * HIRES_WIDTH + x] == 10);
				}
			}
			if (direction < 2) {
				assert(count_color(10) == (80 + HIRES_SCALE) * HIRES_SCALE);
			} else if (direction == 2) {
				assert(count_color(10) > 80 * HIRES_SCALE);
				assert(image[400 * HIRES_WIDTH + 645] == 3);
			} else {
				assert(count_color(10) == HIRES_SCALE * HIRES_SCALE);
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

static void test_line_stroke_clipping(void)
{
	const struct SHAPE3D_HIRES_VECTOR edges[][2] = {{{-641, -40, 640}, {-641, 40, 640}},
													{{640, -40, 640}, {640, 40, 640}},
													{{-40, 401, 640}, {40, 401, 640}},
													{{-40, -400, 640}, {40, -400, 640}}};
	for (legacy_u32 edge = 0; edge < 4; edge++) {
		reset_target();
		queue(RENDER_PRIMITIVE_LINE, 2, edges[edge]);
		shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
		hires_end();
		/* The centerline is outside the image; its stroke still reaches in. */
		assert(count_color(10) == (80 + HIRES_SCALE) * (edge % 2 == 0 ? 2 : 1));
	}

	const struct SHAPE3D_HIRES_VECTOR clipped[][2] = {{{-41, -60, 640}, {-41, 60, 640}},
													  {{-60, 41, 640}, {60, 41, 640}}};
	target.sprite_raster_left = 150;
	target.sprite_raster_right = 170;
	target.sprite_top = 90;
	target.sprite_bottom = 110;
	for (legacy_u32 direction = 0; direction < 2; direction++) {
		reset_target();
		queue(RENDER_PRIMITIVE_LINE, 2, clipped[direction]);
		shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
		hires_end();
		assert(count_color(10) == 20 * HIRES_SCALE * 2);
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
		{-30, -10, 320}, {0, -10, 320}, {0, 10, 320}, {-30, 10, 320}};
	const struct SHAPE3D_HIRES_VECTOR line[] = {{-20, 0, 640}, {20, 0, 640}};
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
		for (legacy_s32 y = 399; y < 403; y++) {
			assert(pixels()[y * HIRES_WIDTH + 639] == 7);
			assert(pixels()[y * HIRES_WIDTH + 640] == 8);
		}
		assert(count_color(8) == 23 * HIRES_SCALE);
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
	const struct VECTOR lines[][2] = {{{-642, -40, 640}, {-642, 40, 640}},
									  {{-40, 402, 640}, {40, 402, 640}}};
	const legacy_u8 primitive[] = {2, 0, 0, 0, 1, 0, 0};
	for (legacy_u32 edge = 0; edge < 2; edge++) {
		prepare_scene(lines[edge], 2, primitive, sizeof(primitive), 1);
		assert(shape3d_transform_and_queue(&scene_instance) == 0);
		assert(polyinfonumpolys == 1);
		shape3d_render_queued_primitives();
		assert(count_color(7) == 80 + HIRES_SCALE);
		assert(pixels()[edge == 0 ? 400 * HIRES_WIDTH : 640] == 7);
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
	assert(count_color(8) > 40);
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

int main(void)
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
	test_line_weight();
	test_near_plane_and_screen_clipping();
	test_line_stroke_clipping();
	test_line_stroke_occlusion();
	test_materials_and_rounded_primitives();
	test_disabled_and_reset();
	test_visible_line_overhang_survives_culling();
	test_thin_polygon_and_attached_detail();
	test_full_polygon_winding();
	test_clipped_polygon_visibility();
	test_wheel_face_and_sort_depth();
	test_crossing_surfaces_use_pixel_depth();
	test_body_panel_occludes_wheel();
	test_joined_track_surfaces();
	test_shared_edge_pixel_coverage();
	test_shared_edge_near_clipping();
	hires_shutdown();
	puts("High-resolution 3D projection and raster tests passed.");
	return 0;
}
