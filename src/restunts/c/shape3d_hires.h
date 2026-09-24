#ifndef RESTUNTS_SHAPE3D_HIRES_H
#define RESTUNTS_SHAPE3D_HIRES_H

#include "math.h"

#if defined(RESTUNTS_SDL3)

/* Keep fractional coordinates from object transformation through rasterization.
 * The original queue remains in logical 320x200 coordinates. */
struct SHAPE3D_HIRES_VECTOR {
	legacy_f64 x;
	legacy_f64 y;
	legacy_f64 z;
};

struct SHAPE3D_HIRES_POINT {
	legacy_f64 x;
	legacy_f64 y;
	legacy_f64 inverse_z;
};

void shape3d_hires_project(const struct SHAPE3D_HIRES_VECTOR *vector,
						   struct SHAPE3D_HIRES_POINT *point);
legacy_u8 shape3d_hires_clip_flags(const struct SHAPE3D_HIRES_VECTOR *vector);
legacy_s32 shape3d_hires_polygon_visible(legacy_u32 index, legacy_s32 cull_backface);
legacy_u32 shape3d_hires_wheel_face(legacy_u32 index);
legacy_f64 shape3d_hires_depth(legacy_u32 index);
void shape3d_hires_reset(void);
/* Scale the stroke with model size; captured per queued primitive. */
void shape3d_hires_set_model_scale(legacy_f64 scale);
enum SHAPE3D_HIRES_DEPTH_MODE {
	SHAPE3D_HIRES_DEPTH_ORDERED,
	SHAPE3D_HIRES_DEPTH_SORTED,
	SHAPE3D_HIRES_DEPTH_BACKGROUND
};
void shape3d_hires_begin_shape(legacy_u32 index, legacy_s32 depth_mode);
void shape3d_hires_queue(legacy_u32 index, legacy_u8 type, legacy_u16 vertex_count,
						 const legacy_u8 *indices, const struct SHAPE3D_HIRES_VECTOR *vertices,
						 legacy_u16 flags);
void shape3d_hires_update_bounds(legacy_u32 index, legacy_u8 type, struct RECTANGLE *rectangle);
/* Record a complete immutable scene, then draw disjoint row bands and join.
 * End returns the background worker count, or zero for the serial fallback. */
void shape3d_hires_batch_begin(void);
legacy_s32 shape3d_hires_batch_end(void);
void shape3d_hires_render(legacy_u32 index, legacy_u8 type, legacy_u16 color,
						  legacy_u16 second_color, legacy_u16 third_color, legacy_u16 pattern_type,
						  legacy_u16 pattern);

#endif

#endif
