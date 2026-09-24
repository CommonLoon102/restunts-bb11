#ifndef RESTUNTS_RENDER_VULKAN_SCENE_H
#define RESTUNTS_RENDER_VULKAN_SCENE_H

#include "legacy.h"

struct HIRES_RASTER_TARGET;

/* Rows are padded to four floats to match the shader's vec4 layout. The
 * inverse matrix retains the fixed-point camera matrix's actual inverse. */
struct RENDER_VULKAN_VIEW {
	legacy_f32 inverse_view[3][4];
	legacy_f32 step_x, step_y, center_x, center_y;
	legacy_f32 footprint_scale, ground;
	legacy_u32 shadow_active, shadow_baked;
};

/* Begin and end form a transaction: a zero result must leave the raster target
 * untouched so the caller can draw the complete batch with its CPU workers.
 * Spans retain submission order, exclusive right edges and pixel-center depth. */
legacy_s32 render_vulkan_scene_begin(const struct HIRES_RASTER_TARGET *target);
void render_vulkan_scene_span(legacy_s32 left, legacy_s32 right, legacy_s32 y, legacy_f64 inverse_z,
							  legacy_f64 depth_step, legacy_u32 family, legacy_s32 depth_mode,
							  legacy_u16 color, legacy_u16 alternate, legacy_u16 pattern,
							  legacy_s32 paint_mode, legacy_s32 depth_test);
legacy_s32 render_vulkan_scene_end(const struct RENDER_VULKAN_VIEW *view);

#endif
