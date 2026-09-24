#ifndef RESTUNTS_HIRES_H
#define RESTUNTS_HIRES_H

#include "legacy.h"

#define HIRES_SCALE 4
#define HIRES_WIDTH 1280
#define HIRES_HEIGHT 800

struct SPRITE;
struct HIRES_SURFACE;

/* Immutable for one joined raster pass. Screen rows must map to disjoint
 * legacy cells; preparation rejects targets that cannot be split safely. */
struct HIRES_RASTER_TARGET {
	struct HIRES_SURFACE *surface;
	legacy_u16 rows[HIRES_HEIGHT / HIRES_SCALE];
	legacy_s32 left, right, top, bottom;
	legacy_f32 *inverse_depth;
	legacy_u32 *depth_family;
	legacy_s32 depth_left, depth_right, depth_top, depth_bottom;
};

/* Each job owns complete legacy rows: top/bottom are multiples of HIRES_SCALE.
 * Contexts must not overlap, and their counters start at zero. */
struct HIRES_RASTER_CONTEXT {
	const struct HIRES_RASTER_TARGET *target;
	legacy_s32 top, bottom;
	legacy_u32 cleared_argb_cells;
};

/* SDL3-only companion pixels. Legacy sprite offsets and resources stay 16-bit. */
void hires_set_enabled(legacy_s32 enabled);
legacy_s32 hires_enabled(void);
legacy_s32 hires_begin(const struct SPRITE *target);
void hires_end(void);
enum HIRES_DEPTH_MODE { HIRES_DEPTH_SURFACE, HIRES_DEPTH_ATTACHED, HIRES_DEPTH_ORDERED };

/* Bounds use high-resolution pixels with exclusive right/bottom edges.
 * Family zero is reserved. Attached decals retain their parent's depth;
 * ordered shapes share a family and retain their nearest supporting depth. */
void hires_depth_begin(legacy_s32 left, legacy_s32 right, legacy_s32 top, legacy_s32 bottom);
legacy_s32 hires_depth_test(legacy_s32 x, legacy_s32 y, legacy_f64 inverse_z, legacy_u32 family,
							legacy_s32 mode);
/* Prepare after hires_begin/hires_depth_begin. Keep the target immutable and
 * its buffers alive; no other drawing may overlap these jobs. After joining,
 * finish once on the main thread with all job counters summed, before hires_end. */
legacy_s32 hires_raster_prepare(struct HIRES_RASTER_TARGET *target);
legacy_s32 hires_raster_depth_test(struct HIRES_RASTER_CONTEXT *context, legacy_s32 x, legacy_s32 y,
								   legacy_f64 inverse_z, legacy_u32 family, legacy_s32 mode);
void hires_raster_pixel(struct HIRES_RASTER_CONTEXT *context, legacy_s32 x, legacy_s32 y,
						legacy_u8 color);
void hires_raster_finish(const struct HIRES_RASTER_TARGET *target, legacy_u32 cleared_argb_cells);
/* Optional full-color artwork uses the same clipping and sprite-copy lifetime.
 * Allocation failure leaves the indexed fallback intact. */
legacy_s32 hires_begin_argb(const struct SPRITE *target);
void hires_argb_pixel(legacy_s32 x, legacy_s32 y, legacy_u32 color);
const legacy_u32 *hires_framebuffer_argb(const legacy_u8 *legacy, const legacy_u32 *palette);
void hires_pixel(legacy_s32 x, legacy_s32 y, legacy_u8 color);
/* Fill all companion samples at logical 320x200 coordinates without changing the legacy byte. */
void hires_fill_pixel(legacy_s32 x, legacy_s32 y, legacy_u8 color);
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
