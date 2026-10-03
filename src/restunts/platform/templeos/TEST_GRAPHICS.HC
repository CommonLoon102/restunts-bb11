// Exercises generated sprite code against the native renderer. No game assets needed.
#include "LOAD.HC"

#define TEMPLE_GRAPHICS_HEADER_BYTES 16
#define TEMPLE_GRAPHICS_POINT_BYTES 4
#define TEMPLE_GRAPHICS_VECTOR_BYTES 6
#define TEMPLE_GRAPHICS_SHAPE_HEADER_BYTES 4
#define TEMPLE_GRAPHICS_SHAPE_VERTEX_COUNT 3
#define TEMPLE_GRAPHICS_SHAPE_MASK_BYTES 4
#define TEMPLE_GRAPHICS_SHAPE_BYTES 40
#define TEMPLE_GRAPHICS_WINDOW_SIZE 16
#define TEMPLE_GRAPHICS_CAPTURE_X 4
#define TEMPLE_GRAPHICS_CAPTURE_Y 3
#define TEMPLE_GRAPHICS_CAPTURE_SIDE 8
#define TEMPLE_GRAPHICS_TEST_COLOR 12
#define TEMPLE_GRAPHICS_CLEAR_COLOR 2
#define TEMPLE_GRAPHICS_POLYGON_HEADER_BYTES 6
#define TEMPLE_GRAPHICS_CLOUD_VERTICES 8
#define TEMPLE_GRAPHICS_TRIANGLE_VERTICES 3

legacy_void TempleGraphicsAssert(legacy_s64 condition, legacy_u8 *message)
{
	if (!condition) {
		PrintErr("Graphics integration: %s\n", message);
		throw('Graphics');
	}
}

legacy_void TempleGraphicsQueueTest()
{
	legacy_u16 expected;
	init_polyinfo();
	polyinfo_reset();
	shape_polygon_predecessor = polygon_list_tail;
	polygon_insertion_cursor = polygon_list_tail;
	shape_polygon_count = 0;
	transshapenumvertscopy = TEMPLE_GRAPHICS_CLOUD_VERTICES;
	TempleGraphicsAssert(!polygon_insert_newest(0, 0), "first primitive fits queue");
	expected = TEMPLE_GRAPHICS_POLYGON_HEADER_BYTES +
			   TEMPLE_GRAPHICS_CLOUD_VERTICES * TEMPLE_GRAPHICS_POINT_BYTES;
	TempleGraphicsAssert(polyinfoptrnext == expected && polyinfonumpolys == 1,
						 "byte vertex count produces exact packed record size");
	transshapenumvertscopy = TEMPLE_GRAPHICS_TRIANGLE_VERTICES;
	TempleGraphicsAssert(!polygon_insert_newest(0, 0), "next primitive fits queue");
	expected += TEMPLE_GRAPHICS_POLYGON_HEADER_BYTES +
				TEMPLE_GRAPHICS_TRIANGLE_VERTICES * TEMPLE_GRAPHICS_POINT_BYTES;
	TempleGraphicsAssert(polyinfoptrnext == expected && polyinfonumpolys == 2,
						 "queue appends subsequent primitives without false overflow");
	polyinfo_reset();
}

legacy_void TempleGraphicsTest()
{
	SPRITE *window;
	SPRITE *background;
	SPRITE context[2];
	POINT2D triangle[3];
	SHAPE3D model_info;
	VECTOR vertex;
	legacy_u8 model[TEMPLE_GRAPHICS_SHAPE_BYTES];
	legacy_u8 *window_bitmap;
	TempleRasterSurface *surface;
	legacy_u32 saved[TEMPLE_GRAPHICS_CAPTURE_SIDE * TEMPLE_GRAPHICS_CAPTURE_SIDE *
					 TEMPLE_RASTER_CELL_PIXELS];
	legacy_u64 x, y, slot, index, offset;
	legacy_u8 mouse_was_shown = ms.show;

	TempleGraphicsAssert(sizeof(SHAPE2D) == TEMPLE_GRAPHICS_HEADER_BYTES, "bitmap header layout");
	TempleGraphicsAssert(sizeof(POINT2D) == TEMPLE_GRAPHICS_POINT_BYTES, "polygon point layout");
	TempleGraphicsAssert(sizeof(VECTOR) == TEMPLE_GRAPHICS_VECTOR_BYTES, "binary vertex layout");
	MemSet(model, 0, sizeof(model));
	model[0] = TEMPLE_GRAPHICS_SHAPE_VERTEX_COUNT;
	model[1] = 1;
	model[2] = 1;
	shape3d_init_shape(model, &model_info);
	TempleGraphicsAssert(model_info.shape3d_vertex_bytes ==
							 model + TEMPLE_GRAPHICS_SHAPE_HEADER_BYTES,
						 "binary vertex pointer");
	offset = TEMPLE_GRAPHICS_SHAPE_HEADER_BYTES +
			 TEMPLE_GRAPHICS_VECTOR_BYTES * TEMPLE_GRAPHICS_SHAPE_VERTEX_COUNT;
	TempleGraphicsAssert(model_info.shape3d_visibility_masks == model + offset,
						 "binary visibility mask pointer");
	TempleGraphicsAssert(model_info.shape3d_front_facing_masks ==
							 model + offset + TEMPLE_GRAPHICS_SHAPE_MASK_BYTES,
						 "binary facing mask pointer");
	TempleGraphicsAssert(model_info.shape3d_primitives ==
							 model + offset + TEMPLE_GRAPHICS_SHAPE_MASK_BYTES * 2,
						 "binary primitive pointer");
	vertex.x = -1;
	vertex.y = 2;
	vertex.z = I16_MIN;
	shape3d_vertex_write(&model_info, 0, &vertex);
	MemSet(&vertex, 0, sizeof(vertex));
	shape3d_vertex_read(&model_info, 0, &vertex);
	TempleGraphicsAssert(vertex.x == -1 && vertex.y == 2 && vertex.z == I16_MIN,
						 "signed binary vertex round trip");
	full_data_initialize;
	init_video_geometry_flags;
	mmgr_init_conventional_arena;
	TempleGraphicsQueueTest();
	dos_video_set_mode_13h;
	TempleGraphicsAssert((temple_video_front->flags & DCF_NO_TRANSPARENTS) != 0 &&
							 (temple_video_back->flags & DCF_NO_TRANSPARENTS) != 0,
						 "opaque frames select native row-copy blitting");
	video_pages_initialize;
	sprite_select_screen;
	window = sprite_make_wnd(TEMPLE_GRAPHICS_WINDOW_SIZE, TEMPLE_GRAPHICS_WINDOW_SIZE, 0);
	background = sprite_make_wnd(TEMPLE_GRAPHICS_CAPTURE_SIDE, TEMPLE_GRAPHICS_CAPTURE_SIDE, 0);
	sprite_select_target(window);
	sprite_save_context(context);
	sprite_select_target(background);
	sprite_restore_context(context);
	TempleGraphicsAssert(drawing_sprite.sprite_bitmapptr == window->sprite_bitmapptr,
						 "restored native bitmap pointer");
	TempleGraphicsAssert(drawing_sprite.sprite_lineofs == window->sprite_lineofs,
						 "restored native row pointer");
	TempleGraphicsAssert(temple_raster_target == TempleRasterFind(window->sprite_bitmapptr),
						 "restored raster target");
	sprite_clear_target(0);
	triangle[0].px = TEMPLE_GRAPHICS_CAPTURE_X;
	triangle[0].py = TEMPLE_GRAPHICS_CAPTURE_Y;
	triangle[1].px = TEMPLE_GRAPHICS_CAPTURE_X + TEMPLE_GRAPHICS_CAPTURE_SIDE;
	triangle[1].py = TEMPLE_GRAPHICS_CAPTURE_Y;
	triangle[2].px = TEMPLE_GRAPHICS_CAPTURE_X;
	triangle[2].py = TEMPLE_GRAPHICS_CAPTURE_Y + TEMPLE_GRAPHICS_CAPTURE_SIDE;
	preRender_default(TEMPLE_GRAPHICS_TEST_COLOR, 3, triangle);
	surface = temple_raster_target;
	index = 0;
	for (y = 0; y < TEMPLE_GRAPHICS_CAPTURE_SIDE; y++) {
		for (x = 0; x < TEMPLE_GRAPHICS_CAPTURE_SIDE; x++) {
			offset = surface->rows[y + TEMPLE_GRAPHICS_CAPTURE_Y] + x + TEMPLE_GRAPHICS_CAPTURE_X;
			for (slot = 0; slot < TEMPLE_RASTER_CELL_PIXELS; slot++) {
				saved[index++] = surface->samples[offset * TEMPLE_RASTER_CELL_PIXELS + slot];
			}
		}
	}
	sprite_clear_shape_alt(background->sprite_bitmapptr, TEMPLE_GRAPHICS_CAPTURE_X,
						   TEMPLE_GRAPHICS_CAPTURE_Y);
	sprite_clear_target(TEMPLE_GRAPHICS_CLEAR_COLOR);
	sprite_putimage(background->sprite_bitmapptr);
	index = 0;
	for (y = 0; y < TEMPLE_GRAPHICS_CAPTURE_SIDE; y++) {
		for (x = 0; x < TEMPLE_GRAPHICS_CAPTURE_SIDE; x++) {
			offset = surface->rows[y + TEMPLE_GRAPHICS_CAPTURE_Y] + x + TEMPLE_GRAPHICS_CAPTURE_X;
			for (slot = 0; slot < TEMPLE_RASTER_CELL_PIXELS; slot++) {
				TempleGraphicsAssert(
					saved[index++] == surface->samples[offset * TEMPLE_RASTER_CELL_PIXELS + slot],
					"generated capture/blit retains every native sample");
			}
		}
	}
	sprite_select_screen;
	sprite_clear_target(0);
	sprite_putimage(window->sprite_bitmapptr);
	TempleVideoPresent;
	window_bitmap = window->sprite_bitmapptr;
	sprite_free_wnd(background);
	sprite_free_wnd(window);
	TempleGraphicsAssert(!TempleRasterFind(window_bitmap), "window raster release");
	TempleVideoShutdown;
	TempleGraphicsAssert(ms.show == mouse_was_shown, "mouse visibility restoration");
	"Generated geometry queue, bitmap, sprite capture/blit and lifetime checks passed.\n";
}
TempleGraphicsTest;
