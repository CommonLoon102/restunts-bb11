#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../c/hires.h"
#include "../c/platform.h"
#include "../c/projection.h"
#include "../c/shape2d.h"
#include "../c/shape3d_hires.h"
#include "../c/shape3d_internal.h"

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

static const unsigned char *pixels(void)
{
	int width;
	int height;
	const unsigned char *result = hires_framebuffer(screen, &width, &height);
	assert(width == HIRES_WIDTH && height == HIRES_HEIGHT);
	return result;
}

static unsigned int count_color(unsigned char color)
{
	const unsigned char *image = pixels();
	unsigned int count = 0;
	for (unsigned int index = 0; index < HIRES_WIDTH * HIRES_HEIGHT; index++) {
		count += image[index] == color;
		if (image[index] == color) {
			int x = (index % HIRES_WIDTH) / HIRES_SCALE;
			int y = (index / HIRES_WIDTH) / HIRES_SCALE;
			assert(x >= bounds.left && x < bounds.right);
			assert(y >= bounds.top && y < bounds.bottom);
		}
	}
	return count;
}

static void queue(legacy_u8 type, unsigned int count, const struct VECTOR *vertices)
{
	legacy_u8 indices[10];
	struct POINT2D projected[10];
	assert(count <= sizeof(indices));
	for (unsigned int index = 0; index < count; index++) {
		indices[index] = (legacy_u8)index;
		struct SHAPE3D_HIRES_POINT point;
		shape3d_hires_project(&vertices[index], &point);
		projected[index].px = (legacy_s16)(point.x / HIRES_SCALE);
		projected[index].py = (legacy_s16)(point.y / HIRES_SCALE);
	}
	shape3d_hires_queue(0, type, count, indices, vertices, projected);
	bounds.left = 320;
	bounds.top = 200;
	bounds.right = 0;
	bounds.bottom = 0;
	shape3d_hires_update_bounds(0, type, &bounds);
}

static void test_projection_and_subpixel_edges(void)
{
	struct VECTOR vector = {1, 1, 300};
	struct SHAPE3D_HIRES_POINT point;
	shape3d_hires_project(&vector, &point);
	assert(point.x > 642 && point.x < 643);
	assert(point.y > 397 && point.y < 398);
	const struct VECTOR triangle[] = {{-100, -60, 300}, {100, -60, 300}, {-100, 60, 300}};
	reset_target();
	queue(RENDER_PRIMITIVE_POLYGON, 3, triangle);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 7, 0, 0, 0, 0);
	hires_end();
	assert(count_color(7) > 40000);
	const unsigned char *image = pixels();
	unsigned int partial_blocks = 0;
	for (int y = 0; y < HIRES_HEIGHT; y += HIRES_SCALE) {
		for (int x = 0; x < HIRES_WIDTH; x += HIRES_SCALE) {
			unsigned int colored = 0;
			for (int row = 0; row < HIRES_SCALE; row++) {
				for (int column = 0; column < HIRES_SCALE; column++) {
					colored += image[(y + row) * HIRES_WIDTH + x + column] == 7;
				}
			}
			partial_blocks += colored != 0 && colored != HIRES_SCALE * HIRES_SCALE;
		}
	}
	assert(partial_blocks > 100);
	/* The detailed pass leaves the legacy image available for compatibility. */
	for (unsigned int index = 0; index < 320 * 200; index++) {
		assert(screen[index] == 3);
	}
}

static void test_near_plane_and_screen_clipping(void)
{
	const struct VECTOR triangle[] = {{-10, -10, 1}, {50, -30, 100}, {0, 50, 100}};
	reset_target();
	queue(RENDER_PRIMITIVE_POLYGON, 3, triangle);
	shape3d_hires_render(0, RENDER_PRIMITIVE_POLYGON, 9, 0, 0, 0, 0);
	hires_end();
	assert(count_color(9) > 100000);

	const struct VECTOR line[] = {{-32768, 0, 12}, {32767, 0, 12}};
	reset_target();
	queue(RENDER_PRIMITIVE_LINE, 2, line);
	shape3d_hires_render(0, RENDER_PRIMITIVE_LINE, 10, 0, 0, 0, 0);
	hires_end();
	assert(count_color(10) == HIRES_WIDTH);
	assert(pixels()[400 * HIRES_WIDTH] == 10);
	assert(pixels()[400 * HIRES_WIDTH + HIRES_WIDTH - 1] == 10);
}

static void test_materials_and_rounded_primitives(void)
{
	const struct VECTOR rectangle[] = {
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

	const struct VECTOR sphere[] = {{0, 0, 300}, {20, 0, 300}};
	reset_target();
	queue(RENDER_PRIMITIVE_SPHERE, 2, sphere);
	shape3d_hires_render(0, RENDER_PRIMITIVE_SPHERE, 11, 0, 0, 0, 0);
	hires_end();
	assert(count_color(11) > 1000);
	assert(pixels()[400 * HIRES_WIDTH + 640] == 11);
	assert(pixels()[380 * HIRES_WIDTH + 640] == 3);

	const struct VECTOR wheel[] = {{-20, 0, 400}, {0, 0, 400},	{-20, 20, 400},
								   {0, 0, 420},	  {20, 0, 420}, {0, 20, 420}};
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
	const struct VECTOR point[] = {{0, 0, 100}};
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
	for (unsigned int row = 0; row < 200; row++) {
		LEGACY_WRITE_U16_LE(rows + row * 2, row * 320);
	}
	projection_center_x = 160;
	projection_center_y = 100;
	projection_focal_length_x = 160;
	projection_focal_length_y = 160;
	test_projection_and_subpixel_edges();
	test_near_plane_and_screen_clipping();
	test_materials_and_rounded_primitives();
	test_disabled_and_reset();
	hires_shutdown();
	puts("High-resolution 3D projection and raster tests passed.");
	return 0;
}
