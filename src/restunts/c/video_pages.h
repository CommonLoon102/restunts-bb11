#ifndef RESTUNTS_VIDEO_PAGES_H
#define RESTUNTS_VIDEO_PAGES_H

#include "legacy.h"

/* Page sprites retain chunky 320-byte rows. Only the raster helpers translate
 * those logical offsets into VGA planes; resource bitmaps remain ordinary RAM. */
#if (defined(__WATCOMC__) && defined(__I86__) && defined(RESTUNTS_FULL)) ||                        \
	defined(RESTUNTS_VGA_TEST)
#define RESTUNTS_HAS_VGA_PAGES 1
void video_pages_initialize(void);
legacy_u8 video_pages_is_active(void);
void video_pages_begin_race(void);
void video_pages_end_race(void);
void video_pages_select_backbuffer(void);
void video_pages_present(void);
legacy_u8 video_pages_is_target(const void far *bitmap);
legacy_u8 video_pages_read_pixel(const legacy_u8 far *bitmap, legacy_u16 offset);
void video_pages_write_pixel(legacy_u8 far *bitmap, legacy_u16 offset, legacy_u8 color);
void video_pages_fill_span(legacy_u8 far *bitmap, legacy_u16 offset, legacy_u16 count,
						   legacy_u8 color);
void video_pages_copy_span(legacy_u8 far *destination, legacy_u16 destination_offset,
						   const legacy_u8 far *source, legacy_u16 source_offset, legacy_u16 count);
void video_pages_raster_span(legacy_u8 far *destination, legacy_u16 destination_offset,
							 const legacy_u8 far *source, legacy_u16 source_offset,
							 legacy_u16 count, legacy_s16 operation, const legacy_u8 far *palette);
void video_pages_pattern_span(legacy_u8 far *bitmap, legacy_u16 offset, legacy_u16 count,
							  legacy_u8 pattern, legacy_u8 color, legacy_u8 alternate_color,
							  legacy_s16 two_colors);
#else
/* Host regressions and replay dumps keep the original packed-memory path. */
static inline void video_pages_initialize(void)
{
}
static inline legacy_u8 video_pages_is_active(void)
{
	return 0;
}
static inline void video_pages_begin_race(void)
{
}
static inline void video_pages_end_race(void)
{
}
static inline void video_pages_select_backbuffer(void)
{
}
static inline void video_pages_present(void)
{
}
static inline legacy_u8 video_pages_is_target(const void far *bitmap)
{
	(void)bitmap;
	return 0;
}
static inline legacy_u8 video_pages_read_pixel(const legacy_u8 far *bitmap, legacy_u16 offset)
{
	return bitmap[offset];
}
static inline void video_pages_write_pixel(legacy_u8 far *bitmap, legacy_u16 offset,
										   legacy_u8 color)
{
	bitmap[offset] = color;
}
static inline void video_pages_fill_span(legacy_u8 far *bitmap, legacy_u16 offset, legacy_u16 count,
										 legacy_u8 color)
{
	while (count-- != 0) {
		bitmap[offset++] = color;
	}
}
static inline void video_pages_raster_span(legacy_u8 far *destination,
										   legacy_u16 destination_offset,
										   const legacy_u8 far *source, legacy_u16 source_offset,
										   legacy_u16 count, legacy_s16 operation,
										   const legacy_u8 far *palette)
{
	while (count-- != 0) {
		legacy_u8 value = source[source_offset++];
		if (operation == 0) {
			destination[destination_offset] &= value;
		} else if (operation == 1) {
			destination[destination_offset] |= value;
		} else if (operation == 3) {
			value = palette[value];
			if (value != 255U) {
				destination[destination_offset] = value;
			}
		} else {
			destination[destination_offset] = value;
		}
		destination_offset++;
	}
}
static inline void video_pages_copy_span(legacy_u8 far *destination, legacy_u16 destination_offset,
										 const legacy_u8 far *source, legacy_u16 source_offset,
										 legacy_u16 count)
{
	video_pages_raster_span(destination, destination_offset, source, source_offset, count, 2, 0);
}
static inline void video_pages_pattern_span(legacy_u8 far *bitmap, legacy_u16 offset,
											legacy_u16 count, legacy_u8 pattern, legacy_u8 color,
											legacy_u8 alternate_color, legacy_s16 two_colors)
{
	while (count-- != 0) {
		pattern = (legacy_u8)((pattern << 1) | (pattern >> 7));
		if ((pattern & 1U) != 0) {
			bitmap[offset] = two_colors != 0 ? alternate_color : color;
		} else if (two_colors != 0) {
			bitmap[offset] = color;
		}
		offset++;
	}
}
#endif

#endif
