#ifndef RESTUNTS_RENDER_VULKAN_H
#define RESTUNTS_RENDER_VULKAN_H

#include "legacy.h"

/* Probe once after SDL video initialization. This creates the device and both
 * compute pipelines; loading a Vulkan library alone is not a capability check.
 * A failed probe remains unavailable for this process until shutdown. */
void render_vulkan_initialize(void);
void render_vulkan_shutdown(void);
legacy_s32 render_vulkan_available(void);
legacy_s32 render_vulkan_enabled(void);
void render_vulkan_set_enabled(legacy_s32 enabled);
/* Successful scene submissions, for diagnostics and backend parity tests. */
legacy_u32 render_vulkan_submission_count(void);

#endif
