#ifndef RESTUNTS_SHAPE3D_SHADOWS_H
#define RESTUNTS_SHAPE3D_SHADOWS_H

#include "shape3d_hires.h"

#if defined(RESTUNTS_SDL3)

struct TRANSFORMEDSHAPE3D;
void shape3d_capture_shadows(const struct TRANSFORMEDSHAPE3D *instance);

/* Presentation setting; defaults to enabled for direct renderer users.
 * Disabling clears transient casters and stops frame capture/sampling, while
 * retaining the baked cache. Enabling takes effect at the next begin call.
 * Explicit static bake calls remain available in either mode. Change this
 * only between rendering passes, after any CPU workers have joined. */
void shape3d_shadows_set_enabled(legacy_s32 enabled);
legacy_s32 shape3d_shadows_enabled(void);

/* Baked static receiver lighting plus a bounded transient light map for
 * animated casters. Runtime samples use camera-relative world axes. */
/* Static geometry is submitted in absolute world coordinates between these
 * calls. The resulting receiver lightmaps survive frame resets. */
legacy_s32 shape3d_shadows_bake_begin(void);
void shape3d_shadows_bake_end(void);
/* Returns one for a matching disk cache, zero for a fresh bake. Cache I/O
 * failure leaves the in-memory bake available. NULL arguments disable disk I/O. */
legacy_s32 shape3d_shadows_bake_end_cached(const char *path, const legacy_u8 track_md5[16]);
void shape3d_shadows_invalidate(void);
legacy_s32 shape3d_shadows_baked(void);
/* Rendering retains the existing 1.5 to 2 tile fade; cached lighting itself
 * remains available throughout the track. */
/* Optional hints belong to one sampling pass. Initialize them to zero after
 * loading or rebaking a track; each rendering worker keeps its own hint. */
legacy_u8 shape3d_shadows_sample_cached_view(legacy_f64 x, legacy_f64 y, legacy_f64 z,
											 legacy_f64 footprint, legacy_u32 *hint);
legacy_u32 shape3d_shadows_baked_bytes(void);
legacy_u8 shape3d_shadows_sample_cached(legacy_f64 x, legacy_f64 y, legacy_f64 z,
										legacy_f64 footprint, legacy_u32 *hint);
void shape3d_shadows_begin(const struct VECTOR *camera_origin);
void shape3d_shadows_reset(void);
void shape3d_shadows_shutdown(void);
/* True during an explicit static bake, or an enabled frame capture. */
legacy_s32 shape3d_shadows_active(void);
legacy_f64 shape3d_shadows_ground_height(void);
void shape3d_shadows_add_polygon(const struct SHAPE3D_HIRES_VECTOR *vertices, legacy_u32 count,
								 legacy_s32 grille);
/* Return translucent black opacity. An inactive map always returns zero. */
legacy_u8 shape3d_shadows_sample(legacy_f64 x, legacy_f64 y, legacy_f64 z);
/* The unnormalized world-space receiver normal prevents self shading on
 * sloping roads and walls when filtering neighboring light-map texels. */
legacy_u8 shape3d_shadows_sample_plane(legacy_f64 x, legacy_f64 y, legacy_f64 z,
									   legacy_f64 normal_x, legacy_f64 normal_y,
									   legacy_f64 normal_z);

/* Borrowed uint32 storage buffers for the Vulkan shadow sampler. Static data
 * only needs uploading when static_revision changes (zero is never returned).
 * The frame buffer contains camera state and bounded dynamic map rectangles;
 * its complete frame_bytes must be uploaded each frame. Both byte sizes are
 * multiples of four. Data uses fp32 and explicitly packed lightmap bytes.
 * The pointers remain valid until the next export, invalidation or shutdown.
 * Call after the final shadow caster is captured, outside CPU sampling work.
 * Returns zero on allocation failure or active lighting without a baked cache;
 * the caller must then render this frame using the existing CPU path. Inactive
 * lighting succeeds and exports a zero-active frame. The ABI is private to
 * platform/sdl3/shaders/shadows.glsl; never persist it as a disk cache. */
struct SHAPE3D_SHADOWS_GPU_DATA {
	const legacy_u32 *static_words;
	const legacy_u32 *frame_words;
	legacy_u32 static_bytes;
	legacy_u32 frame_bytes;
	legacy_u32 static_revision;
};
legacy_s32 shape3d_shadows_gpu_export(struct SHAPE3D_SHADOWS_GPU_DATA *output);

#endif

#endif
