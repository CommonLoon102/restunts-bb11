#ifndef RESTUNTS_SHAPE3D_HIRES_H
#define RESTUNTS_SHAPE3D_HIRES_H

#include "math.h"

#if defined(RESTUNTS_SDL3)

/* Keep fractional coordinates until rasterization, including wheel and sphere
 * construction. The original queue remains in logical 320x200 coordinates. */
struct SHAPE3D_HIRES_POINT {
	double x;
	double y;
	double inverse_z;
};

void shape3d_hires_project(const struct VECTOR *vector, struct SHAPE3D_HIRES_POINT *point);
legacy_u8 shape3d_hires_clip_flags(const struct VECTOR *vector);
int shape3d_hires_polygon_visible(legacy_u16 index, int cull_backface);
unsigned int shape3d_hires_wheel_face(legacy_u16 index);
void shape3d_hires_reset(void);
void shape3d_hires_begin_shape(legacy_u16 index, int depth_test);
void shape3d_hires_queue(legacy_u16 index, legacy_u8 type, legacy_u16 vertex_count,
						 const legacy_u8 *indices, const struct VECTOR *vertices, legacy_u16 flags);
void shape3d_hires_update_bounds(legacy_u16 index, legacy_u8 type, struct RECTANGLE *rectangle);
void shape3d_hires_render(legacy_u16 index, legacy_u8 type, legacy_u16 color,
						  legacy_u16 second_color, legacy_u16 third_color, legacy_u16 pattern_type,
						  legacy_u16 pattern);

#endif

#endif
