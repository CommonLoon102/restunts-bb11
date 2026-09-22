#ifndef RESTUNTS_HIRES_H
#define RESTUNTS_HIRES_H

#include "legacy.h"

#define HIRES_SCALE 4
#define HIRES_WIDTH 1280
#define HIRES_HEIGHT 800

struct SPRITE;

/* SDL3-only companion pixels. Legacy sprite offsets and resources stay 16-bit. */
void hires_set_enabled(legacy_s32 enabled);
legacy_s32 hires_enabled(void);
legacy_s32 hires_begin(const struct SPRITE *target);
void hires_end(void);
/* Bounds use high-resolution pixels with exclusive right/bottom edges.
 * Family zero is reserved; attached fragments may overlay their own parent. */
void hires_depth_begin(legacy_s32 left, legacy_s32 right, legacy_s32 top, legacy_s32 bottom);
legacy_s32 hires_depth_test(legacy_s32 x, legacy_s32 y, legacy_f64 inverse_z, legacy_u16 family,
							legacy_s32 attached);
void hires_pixel(legacy_s32 x, legacy_s32 y, legacy_u8 color);
void hires_write(const legacy_u8 *base, legacy_u16 offset, legacy_u8 color);
void hires_raster(const legacy_u8 *destination, legacy_u16 destination_offset,
				  const legacy_u8 *source, legacy_u16 source_offset, legacy_u16 count,
				  legacy_s16 operation, const legacy_u8 *palette);
void hires_forget(const void *base);
void hires_forget_range(const void *base, legacy_u32 size);
const legacy_u8 *hires_framebuffer(const legacy_u8 *legacy, legacy_s32 *width, legacy_s32 *height);
legacy_u32 hires_generation(void);
void hires_shutdown(void);

#endif
