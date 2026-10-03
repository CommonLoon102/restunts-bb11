#ifndef RESTUNTS_TEMPLE_RASTER_HH
#define RESTUNTS_TEMPLE_RASTER_HH

#include "LEGACY.HH"

extern legacy_void TempleRasterRegisterSprite(SPRITE *sprite);
extern legacy_void TempleRasterSelect(SPRITE *sprite);
extern legacy_void TempleRasterForget(legacy_void *bitmap);
extern legacy_void TempleRasterForgetRange(legacy_u8 *begin, legacy_u32 size);
extern legacy_void TempleRasterXorPixel(legacy_u8 *bitmap, legacy_u16 offset, legacy_u8 mask);
extern legacy_void TempleRasterShutdown();
extern legacy_void TempleRasterSource(legacy_void *shape);
extern legacy_void *TempleRasterResource(legacy_void *resource);
extern legacy_void *TempleRasterResourceParsed(legacy_void *resource);
extern legacy_void TempleRasterPresent(CDC *dc);
extern legacy_u8 TempleRasterScreenDirty();
extern legacy_void TempleRasterScreenPresented();

extern legacy_void video_pages_initialize();
extern legacy_u8 video_pages_is_active();
extern legacy_void video_pages_begin_race();
extern legacy_void video_pages_end_race();
extern legacy_void video_pages_select_backbuffer();
extern legacy_void video_pages_present();
extern legacy_u8 video_pages_is_target(legacy_void *bitmap);
extern legacy_u8 video_pages_uses_raster_hooks(legacy_void *bitmap);
extern legacy_u8 video_pages_read_pixel(legacy_u8 *bitmap, legacy_u16 offset);
extern legacy_void video_pages_write_pixel(legacy_u8 *bitmap, legacy_u16 offset, legacy_u8 color);
extern legacy_void video_pages_fill_span(legacy_u8 *bitmap, legacy_u16 offset, legacy_u16 count,
										 legacy_u8 color);
extern legacy_void video_pages_copy_span(legacy_u8 *destination, legacy_u16 destination_offset,
										 legacy_u8 *source, legacy_u16 source_offset,
										 legacy_u16 count);
extern legacy_void video_pages_raster_span(legacy_u8 *destination, legacy_u16 destination_offset,
										   legacy_u8 *source, legacy_u16 source_offset,
										   legacy_u16 count, legacy_s16 operation,
										   legacy_u8 *palette);
extern legacy_void video_pages_pattern_span(legacy_u8 *bitmap, legacy_u16 offset, legacy_u16 count,
											legacy_u8 pattern, legacy_u8 color,
											legacy_u8 alternate_color, legacy_s16 two_colors);

#endif
