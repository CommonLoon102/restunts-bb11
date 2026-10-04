/* Shared HolyC/host checks for DOS UI line pixels and native line detail. */
#define TEST_LINE_LOW 1
#define TEST_LINE_HIGH 7
#define TEST_LINE_CLIP_LOW 2
#define TEST_LINE_CLIP_HIGH 6
#define TEST_LINE_COLOR 4
#define TEST_LINE_SEGMENT_COUNT 5
#define TEST_LINE_START_X 0
#define TEST_LINE_START_Y 1
#define TEST_LINE_END_X 2
#define TEST_LINE_END_Y 3
#define TEST_LINE_COORDINATE_COUNT 4
#define TEST_LINE_HORIZONTAL 0
#define TEST_LINE_BEVEL_LAYERS 3
#define TEST_LINE_CHECK_BOUND ((TEST_LINE_HIGH + TEST_LINE_BEVEL_LAYERS + 1) * TEMPLE_RENDER_SCALE)

legacy_s16 test_line_coordinates[TEST_LINE_SEGMENT_COUNT * TEST_LINE_COORDINATE_COUNT] = {
	TEST_LINE_LOW, TEST_LINE_LOW,  TEST_LINE_HIGH, TEST_LINE_LOW,  // Horizontal bevel
	TEST_LINE_LOW, TEST_LINE_LOW,  TEST_LINE_LOW,  TEST_LINE_HIGH, // Vertical
	TEST_LINE_LOW, TEST_LINE_LOW,  TEST_LINE_HIGH, TEST_LINE_HIGH, // Descending diagonal
	TEST_LINE_LOW, TEST_LINE_HIGH, TEST_LINE_HIGH, TEST_LINE_LOW,  // Ascending diagonal
	TEST_LINE_LOW, TEST_LINE_LOW,  TEST_LINE_LOW,  TEST_LINE_LOW}; // Single point

legacy_void TestLineCoverage(CDC *dc)
{
	SPRITE saved_sprite;
	legacy_s64 native;
	legacy_s64 scale;
	legacy_s64 clipped;
	legacy_s64 reverse;
	legacy_s64 segment;
	legacy_s64 layer;
	legacy_s64 layers;
	legacy_s64 start_x;
	legacy_s64 start_y;
	legacy_s64 end_x;
	legacy_s64 end_y;
	legacy_s64 x;
	legacy_s64 y;
	legacy_s64 source_x;
	legacy_s64 source_y;
	legacy_s64 line_y;
	legacy_u8 expected;
	MemCpy(&saved_sprite, &drawing_sprite, sizeof(SPRITE));
	for (native = FALSE; native <= TRUE; native++) {
		TempleRasterNative(native);
		scale = TempleRasterCoordinateScale();
		for (clipped = FALSE; clipped <= TRUE; clipped++) {
			drawing_sprite.sprite_top = 0;
			drawing_sprite.sprite_bottom = TEMPLE_LOGICAL_HEIGHT;
			drawing_sprite.sprite_raster_left = 0;
			drawing_sprite.sprite_raster_right = TEMPLE_LOGICAL_WIDTH;
			if (clipped) {
				drawing_sprite.sprite_top = TEST_LINE_CLIP_LOW;
				drawing_sprite.sprite_bottom = TEST_LINE_CLIP_HIGH;
				drawing_sprite.sprite_raster_left = TEST_LINE_CLIP_LOW;
				drawing_sprite.sprite_raster_right = TEST_LINE_CLIP_HIGH;
			}
			for (segment = 0; segment < TEST_LINE_SEGMENT_COUNT; segment++) {
				start_x =
					test_line_coordinates[segment * TEST_LINE_COORDINATE_COUNT + TEST_LINE_START_X];
				start_y =
					test_line_coordinates[segment * TEST_LINE_COORDINATE_COUNT + TEST_LINE_START_Y];
				end_x =
					test_line_coordinates[segment * TEST_LINE_COORDINATE_COUNT + TEST_LINE_END_X];
				end_y =
					test_line_coordinates[segment * TEST_LINE_COORDINATE_COUNT + TEST_LINE_END_Y];
				layers = 1;
				if (segment == TEST_LINE_HORIZONTAL) {
					/* Adjacent UI lines must form the solid three-pixel button bevel. */
					layers = TEST_LINE_BEVEL_LAYERS;
				}
				for (reverse = FALSE; reverse <= TRUE; reverse++) {
					video_pages_fill_span(drawing_sprite.sprite_bitmapptr, 0,
										  TEMPLE_LOGICAL_WIDTH * TEMPLE_LOGICAL_HEIGHT, BLACK);
					for (layer = 0; layer < layers; layer++) {
						if (reverse) {
							preRender_line(end_x, end_y + layer, start_x, start_y + layer,
										   TEST_LINE_COLOR);
						} else {
							preRender_line(start_x, start_y + layer, end_x, end_y + layer,
										   TEST_LINE_COLOR);
						}
					}
					TempleRasterPresent(dc);
					for (y = 0; y < TEST_LINE_CHECK_BOUND; y++) {
						for (x = 0; x < TEST_LINE_CHECK_BOUND; x++) {
							expected = BLACK;
							source_x = x / scale;
							source_y = y / scale;
							for (layer = 0; layer < layers; layer++) {
								line_y = source_y - layer;
								/* Bounding boxes and collinearity give an independent oracle
								 * for horizontal, vertical, diagonal and single-point lines. */
								if (source_x >= MinI64(start_x, end_x) &&
									source_x <= MaxI64(start_x, end_x) &&
									line_y >= MinI64(start_y, end_y) &&
									line_y <= MaxI64(start_y, end_y) &&
									(source_x - start_x) * (end_y - start_y) ==
										(line_y - start_y) * (end_x - start_x) &&
									x >=
										drawing_sprite.sprite_raster_left * TEMPLE_RASTER_X_SCALE &&
									x < drawing_sprite.sprite_raster_right *
											TEMPLE_RASTER_X_SCALE &&
									y >= drawing_sprite.sprite_top * TEMPLE_RASTER_Y_SCALE &&
									y < drawing_sprite.sprite_bottom * TEMPLE_RASTER_Y_SCALE) {
									expected = TEST_LINE_COLOR;
								}
							}
							TestAssert(TestRasterPixel(dc, x, y) == expected);
						}
					}
				}
			}
		}
	}
	TempleRasterNative(FALSE);
	MemCpy(&drawing_sprite, &saved_sprite, sizeof(SPRITE));
}
