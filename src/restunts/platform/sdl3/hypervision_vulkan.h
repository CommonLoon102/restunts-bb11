#ifndef RESTUNTS_HYPERVISION_VULKAN_H
#define RESTUNTS_HYPERVISION_VULKAN_H

#include "../../c/hypervision.h"

#define HV_VULKAN_COLOR_COMPONENTS 4U
#define HV_VULKAN_COLOR_INDEX 0U
#define HV_VULKAN_COVERAGE_INDEX 1U

/* Full-width row-major buffers, valid until the next render or shutdown. The
 * color buffer has four bytes per pixel: palette index, coverage, zero, zero.
 * Uncovered pixels have zero coverage and must not replace companion artwork. */
struct HV_VULKAN_RESULT {
	const legacy_u8 *color_coverage;
	const legacy_f32 *inverse_depth;
	const legacy_u32 *family;
	legacy_s32 width, height;
};

legacy_s32 hv_vulkan_initialize(void);
void hv_vulkan_shutdown(void);
const legacy_char *hv_vulkan_name(void);
const legacy_char *hv_vulkan_driver(void);
legacy_s32 hv_vulkan_hardware_accelerated(void);
/* Failure never writes the target or result. Incremental depth is unsupported;
 * the caller retains its CPU command stream and can rasterize the entire batch. */
legacy_s32 hv_vulkan_render(const struct HYPERVISION_POLYGON *polygons, legacy_u32 count,
							const struct HIRES_RASTER_TARGET *target,
							const struct HIRES_DEPTH_PLANE *ground, legacy_s32 preserve_depth,
							struct HV_VULKAN_RESULT *result);

#endif
