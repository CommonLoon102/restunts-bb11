#include "../../c/render_vulkan.h"
#include "../../c/render_vulkan_scene.h"

#if defined(RESTUNTS_VULKAN) && !defined(__DJGPP__)

#include <SDL3/SDL.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "../../c/hires.h"
#include "../../c/shape3d_shadows.h"
#include "shaders/vulkan_shaders.h"

/* One warp processes a 32-pixel row segment. Its list contains only spans
 * intersecting that segment, in the original painter order. No atomics or
 * inter-workgroup synchronization are needed for depth-family semantics. */
#define VULKAN_TILE_WIDTH 32U
#define VULKAN_RASTER_THREADS 128U
#define VULKAN_TILE_COLUMNS (HIRES_WIDTH / VULKAN_TILE_WIDTH)
#define VULKAN_TILE_COUNT (VULKAN_TILE_COLUMNS * HIRES_HEIGHT)
/* Vulkan 1.0 guarantees at least this storage-buffer descriptor range. SDL
 * does not expose a larger per-device range; oversized scenes use the CPU. */
#define VULKAN_MAX_BUFFER_BYTES (128U * 1024U * 1024U)

struct VULKAN_SPAN {
	legacy_s32 left, right, y;
	legacy_u32 family;
	legacy_f32 inverse_z, depth_step;
	legacy_u32 depth_mode, depth_test;
	legacy_u32 color, alternate, pattern, paint_mode;
};

struct VULKAN_UNIFORMS {
	legacy_s32 left, top, right, bottom;
	struct RENDER_VULKAN_VIEW view;
};

struct VULKAN_BUFFER {
	SDL_GPUBuffer *gpu;
	SDL_GPUTransferBuffer *upload;
	legacy_u32 capacity;
};

static SDL_GPUDevice *device;
static SDL_GPUComputePipeline *raster_pipeline, *shadow_pipeline;
static struct VULKAN_BUFFER span_buffer, tile_buffer, static_buffer, frame_buffer;
static SDL_GPUBuffer *pixel_buffer;
static SDL_GPUTransferBuffer *download;
static legacy_s32 probed, available, enabled, capture_failed;
static legacy_u32 submissions, uploaded_static_revision;
static struct VULKAN_SPAN *spans;
static size_t span_count, span_capacity;
static legacy_u32 *tile_words;
static size_t tile_capacity;
static legacy_u32 tile_counts[VULKAN_TILE_COUNT];
static legacy_u32 tile_cursors[VULKAN_TILE_COUNT];
static const struct HIRES_RASTER_TARGET *capture_target;

/* Layouts are shared with std430/std140 shaders and require no compiler packing. */
typedef legacy_s8 vulkan_span_layout[(sizeof(struct VULKAN_SPAN) == 48) ? 1 : -1];
typedef legacy_s8 vulkan_pixel_layout[(sizeof(struct HIRES_RASTER_SAMPLE) == 12) ? 1 : -1];
typedef legacy_s8 vulkan_uniform_layout[(sizeof(struct VULKAN_UNIFORMS) == 96) ? 1 : -1];

static legacy_s32 environment_enabled(const char *name)
{
	const char *value = SDL_getenv(name);
	return value != NULL && strcmp(value, "1") == 0;
}

static void release_buffer(struct VULKAN_BUFFER *buffer)
{
	if (buffer->gpu != NULL) {
		SDL_ReleaseGPUBuffer(device, buffer->gpu);
	}
	if (buffer->upload != NULL) {
		SDL_ReleaseGPUTransferBuffer(device, buffer->upload);
	}
	memset(buffer, 0, sizeof(*buffer));
}

static void release_device(void)
{
	if (device != NULL) {
		SDL_WaitForGPUIdle(device);
		release_buffer(&span_buffer);
		release_buffer(&tile_buffer);
		release_buffer(&static_buffer);
		release_buffer(&frame_buffer);
		if (pixel_buffer != NULL) {
			SDL_ReleaseGPUBuffer(device, pixel_buffer);
		}
		if (download != NULL) {
			SDL_ReleaseGPUTransferBuffer(device, download);
		}
		if (raster_pipeline != NULL) {
			SDL_ReleaseGPUComputePipeline(device, raster_pipeline);
		}
		if (shadow_pipeline != NULL) {
			SDL_ReleaseGPUComputePipeline(device, shadow_pipeline);
		}
		SDL_DestroyGPUDevice(device);
	}
	device = NULL;
	pixel_buffer = NULL;
	download = NULL;
	raster_pipeline = shadow_pipeline = NULL;
	uploaded_static_revision = 0;
	available = enabled = 0;
}

static void disable_after_failure(const char *operation)
{
	SDL_LogWarn(SDL_LOG_CATEGORY_RENDER, "Vulkan %s failed; using CPU rendering: %s", operation,
				SDL_GetError());
	/* Keep resources alive until shutdown in case a lost device still owns a
	 * submitted command. Never retry device/pipeline creation on an F10 press. */
	available = enabled = 0;
	capture_target = NULL;
}

static SDL_GPUComputePipeline *create_pipeline(const legacy_u32 *code, size_t size, legacy_u32 x,
											   legacy_u32 y)
{
	SDL_GPUComputePipelineCreateInfo info = {0};
	info.code = (const legacy_u8 *)code;
	info.code_size = size;
	info.entrypoint = "main";
	info.format = SDL_GPU_SHADERFORMAT_SPIRV;
	info.num_readonly_storage_buffers = 2;
	info.num_readwrite_storage_buffers = 1;
	info.num_uniform_buffers = 1;
	info.threadcount_x = x;
	info.threadcount_y = y;
	info.threadcount_z = 1;
	return SDL_CreateGPUComputePipeline(device, &info);
}

void render_vulkan_initialize(void)
{
	if (probed) {
		return;
	}
	probed = 1;
	if (environment_enabled("RESTUNTS_VULKAN_DISABLE")) {
		return;
	}
	SDL_PropertiesID properties = SDL_CreateProperties();
	if (properties == 0) {
		return;
	}
	SDL_SetStringProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_NAME_STRING, "vulkan");
	SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN, true);
	SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN,
						   environment_enabled("RESTUNTS_VULKAN_DEBUG"));
	SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_VERBOSE_BOOLEAN, false);
	/* A CPU Vulkan driver would compound the CPU bottleneck. It is useful only
	 * for explicit shader validation on machines without a passed-through GPU. */
	SDL_SetBooleanProperty(properties,
						   SDL_PROP_GPU_DEVICE_CREATE_VULKAN_REQUIRE_HARDWARE_ACCELERATION_BOOLEAN,
						   !environment_enabled("RESTUNTS_VULKAN_SOFTWARE"));
	SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_CLIP_DISTANCE_BOOLEAN,
						   false);
	SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_DEPTH_CLAMPING_BOOLEAN,
						   false);
	SDL_SetBooleanProperty(
		properties, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_INDIRECT_DRAW_FIRST_INSTANCE_BOOLEAN, false);
	SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_ANISOTROPY_BOOLEAN,
						   false);
	device = SDL_CreateGPUDeviceWithProperties(properties);
	SDL_DestroyProperties(properties);
	if (device == NULL) {
		SDL_Log("Vulkan renderer unavailable: %s", SDL_GetError());
		return;
	}
	raster_pipeline = create_pipeline(restunts_vulkan_raster_spv,
									  sizeof(restunts_vulkan_raster_spv), VULKAN_RASTER_THREADS, 1);
	shadow_pipeline =
		create_pipeline(restunts_vulkan_shade_spv, sizeof(restunts_vulkan_shade_spv), 8, 8);
	legacy_u32 pixel_bytes = HIRES_WIDTH * HIRES_HEIGHT * sizeof(struct HIRES_RASTER_SAMPLE);
	SDL_GPUBufferCreateInfo pixels = {SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ |
										  SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE,
									  pixel_bytes, 0};
	pixel_buffer = SDL_CreateGPUBuffer(device, &pixels);
	SDL_GPUTransferBufferCreateInfo transfer = {SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD, pixel_bytes,
												0};
	download = SDL_CreateGPUTransferBuffer(device, &transfer);
	if (raster_pipeline == NULL || shadow_pipeline == NULL || pixel_buffer == NULL ||
		download == NULL) {
		SDL_Log("Vulkan renderer unavailable: %s", SDL_GetError());
		release_device();
		return;
	}
	available = 1;
	SDL_Log("Vulkan renderer ready: %s",
			SDL_GetStringProperty(SDL_GetGPUDeviceProperties(device),
								  SDL_PROP_GPU_DEVICE_NAME_STRING, "Vulkan device"));
}

void render_vulkan_shutdown(void)
{
	release_device();
	free(spans);
	free(tile_words);
	spans = NULL;
	tile_words = NULL;
	span_count = span_capacity = tile_capacity = 0;
	capture_target = NULL;
	probed = 0;
	submissions = 0;
}

legacy_s32 render_vulkan_available(void)
{
	return available;
}

legacy_s32 render_vulkan_enabled(void)
{
	return enabled;
}

void render_vulkan_set_enabled(legacy_s32 value)
{
	enabled = value != 0 && available;
}

legacy_u32 render_vulkan_submission_count(void)
{
	return submissions;
}

static legacy_s32 reserve_host(void **memory, size_t *capacity, size_t count, size_t size)
{
	if (count <= *capacity) {
		return 1;
	}
	if (count > VULKAN_MAX_BUFFER_BYTES / size) {
		return 0;
	}
	size_t grown = *capacity != 0 ? *capacity : 4096;
	while (grown < count) {
		grown *= 2;
	}
	if (grown > VULKAN_MAX_BUFFER_BYTES / size) {
		grown = VULKAN_MAX_BUFFER_BYTES / size;
	}
	void *next = realloc(*memory, grown * size);
	if (next == NULL) {
		return 0;
	}
	*memory = next;
	*capacity = grown;
	return 1;
}

legacy_s32 render_vulkan_scene_begin(const struct HIRES_RASTER_TARGET *target)
{
	capture_target = NULL;
	if (!enabled || target == NULL || target->inverse_depth == NULL ||
		target->depth_family == NULL || target->left != target->depth_left ||
		target->right != target->depth_right || target->top != target->depth_top ||
		target->bottom != target->depth_bottom ||
		((target->left | target->right | target->top | target->bottom) & 3) != 0) {
		return 0;
	}
	/* A normal scene starts with empty depth. Unusual callers that continue an
	 * existing raster pass use the CPU path, preserving their prior depth. */
	for (legacy_s32 y = target->top; y < target->bottom; y++) {
		for (legacy_s32 x = target->left; x < target->right; x++) {
			if (target->depth_family[(size_t)y * HIRES_WIDTH + x] != 0) {
				return 0;
			}
		}
	}
	capture_target = target;
	capture_failed = 0;
	span_count = 0;
	return 1;
}

void render_vulkan_scene_span(legacy_s32 left, legacy_s32 right, legacy_s32 y, legacy_f64 inverse_z,
							  legacy_f64 depth_step, legacy_u32 family, legacy_s32 depth_mode,
							  legacy_u16 color, legacy_u16 alternate, legacy_u16 pattern,
							  legacy_s32 paint_mode, legacy_s32 depth_test)
{
	const struct HIRES_RASTER_TARGET *target = capture_target;
	if (target == NULL || capture_failed || y < target->top || y >= target->bottom ||
		(depth_test && family == 0)) {
		return;
	}
	if (right > target->right) {
		right = target->right;
	}
	if (left < target->left) {
		inverse_z += (target->left - left) * depth_step;
		left = target->left;
	}
	if (left >= right) {
		return;
	}
	if (!reserve_host((void **)&spans, &span_capacity, span_count + 1, sizeof(*spans))) {
		capture_failed = 1;
		return;
	}
	spans[span_count++] = (struct VULKAN_SPAN){left,
											   right,
											   y,
											   family,
											   (legacy_f32)inverse_z,
											   (legacy_f32)depth_step,
											   (legacy_u32)depth_mode,
											   depth_test != 0,
											   color & 255U,
											   alternate & 255U,
											   pattern,
											   (legacy_u32)paint_mode};
}

static legacy_s32 build_tiles(legacy_u32 *bytes)
{
	memset(tile_counts, 0, sizeof(tile_counts));
	size_t references = 0;
	for (size_t i = 0; i < span_count; i++) {
		const struct VULKAN_SPAN *span = &spans[i];
		legacy_u32 first = span->left / VULKAN_TILE_WIDTH;
		legacy_u32 last = (span->right - 1) / VULKAN_TILE_WIDTH;
		references += last - first + 1;
		for (legacy_u32 x = first; x <= last; x++) {
			tile_counts[span->y * VULKAN_TILE_COLUMNS + x]++;
		}
	}
	size_t words = VULKAN_TILE_COUNT * 2U + references;
	if (!reserve_host((void **)&tile_words, &tile_capacity, words, sizeof(*tile_words))) {
		return 0;
	}
	legacy_u32 cursor = VULKAN_TILE_COUNT * 2U;
	for (legacy_u32 tile = 0; tile < VULKAN_TILE_COUNT; tile++) {
		tile_words[tile * 2] = tile_cursors[tile] = cursor;
		tile_words[tile * 2 + 1] = tile_counts[tile];
		cursor += tile_counts[tile];
	}
	for (size_t i = 0; i < span_count; i++) {
		const struct VULKAN_SPAN *span = &spans[i];
		for (legacy_u32 x = span->left / VULKAN_TILE_WIDTH;
			 x <= (legacy_u32)(span->right - 1) / VULKAN_TILE_WIDTH; x++) {
			legacy_u32 tile = span->y * VULKAN_TILE_COLUMNS + x;
			tile_words[tile_cursors[tile]++] = (legacy_u32)i;
		}
	}
	*bytes = (legacy_u32)(words * sizeof(*tile_words));
	return 1;
}

static legacy_s32 prepare_upload(struct VULKAN_BUFFER *buffer, const void *data, legacy_u32 bytes)
{
	if (bytes > VULKAN_MAX_BUFFER_BYTES) {
		return 0;
	}
	if (bytes > buffer->capacity) {
		legacy_u32 capacity = buffer->capacity != 0 ? buffer->capacity : 4096;
		while (capacity < bytes) {
			capacity *= 2;
		}
		release_buffer(buffer);
		SDL_GPUBufferCreateInfo info = {SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ, capacity, 0};
		SDL_GPUTransferBufferCreateInfo upload = {SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD, capacity, 0};
		buffer->gpu = SDL_CreateGPUBuffer(device, &info);
		buffer->upload = SDL_CreateGPUTransferBuffer(device, &upload);
		if (buffer->gpu == NULL || buffer->upload == NULL) {
			release_buffer(buffer);
			return 0;
		}
		buffer->capacity = capacity;
	}
	void *mapped = SDL_MapGPUTransferBuffer(device, buffer->upload, false);
	if (mapped == NULL) {
		return 0;
	}
	memcpy(mapped, data, bytes);
	SDL_UnmapGPUTransferBuffer(device, buffer->upload);
	return 1;
}

static void record_upload(SDL_GPUCopyPass *pass, const struct VULKAN_BUFFER *buffer,
						  legacy_u32 bytes)
{
	SDL_GPUTransferBufferLocation source = {buffer->upload, 0};
	SDL_GPUBufferRegion destination = {buffer->gpu, 0, bytes};
	SDL_UploadToGPUBuffer(pass, &source, &destination, false);
}

legacy_s32 render_vulkan_scene_end(const struct RENDER_VULKAN_VIEW *view)
{
	const struct HIRES_RASTER_TARGET *target = capture_target;
	capture_target = NULL;
	if (target == NULL || capture_failed || !enabled) {
		return 0;
	}
	legacy_u32 tile_bytes;
	struct SHAPE3D_SHADOWS_GPU_DATA shadows;
	if (!build_tiles(&tile_bytes) || !shape3d_shadows_gpu_export(&shadows)) {
		return 0;
	}
	/* Bind a valid dummy span even when a scene only contains sky/ground. */
	struct VULKAN_SPAN empty = {0};
	legacy_u32 span_bytes =
		span_count != 0 ? (legacy_u32)(span_count * sizeof(*spans)) : sizeof(empty);
	legacy_s32 new_static =
		uploaded_static_revision != shadows.static_revision || static_buffer.gpu == NULL;
	if (!prepare_upload(&span_buffer, span_count != 0 ? spans : &empty, span_bytes) ||
		!prepare_upload(&tile_buffer, tile_words, tile_bytes) ||
		!prepare_upload(&frame_buffer, shadows.frame_words, shadows.frame_bytes) ||
		(new_static &&
		 !prepare_upload(&static_buffer, shadows.static_words, shadows.static_bytes))) {
		disable_after_failure("buffer upload");
		return 0;
	}
	SDL_GPUCommandBuffer *command = SDL_AcquireGPUCommandBuffer(device);
	if (command == NULL) {
		disable_after_failure("command acquisition");
		return 0;
	}
	SDL_GPUCopyPass *copy = SDL_BeginGPUCopyPass(command);
	if (copy == NULL) {
		SDL_CancelGPUCommandBuffer(command);
		disable_after_failure("copy pass");
		return 0;
	}
	record_upload(copy, &span_buffer, span_bytes);
	record_upload(copy, &tile_buffer, tile_bytes);
	record_upload(copy, &frame_buffer, shadows.frame_bytes);
	if (new_static) {
		record_upload(copy, &static_buffer, shadows.static_bytes);
	}
	SDL_EndGPUCopyPass(copy);
	struct VULKAN_UNIFORMS uniforms = {target->left, target->top, target->right, target->bottom,
									   *view};
	SDL_PushGPUComputeUniformData(command, 0, &uniforms, sizeof(uniforms));
	SDL_GPUStorageBufferReadWriteBinding output = {pixel_buffer, false, 0, 0, 0};
	SDL_GPUComputePass *compute = SDL_BeginGPUComputePass(command, NULL, 0, &output, 1);
	if (compute == NULL) {
		SDL_CancelGPUCommandBuffer(command);
		disable_after_failure("raster pass");
		return 0;
	}
	SDL_GPUBuffer *inputs[] = {span_buffer.gpu, tile_buffer.gpu};
	SDL_BindGPUComputePipeline(compute, raster_pipeline);
	SDL_BindGPUComputeStorageBuffers(compute, 0, inputs, 2);
	SDL_DispatchGPUCompute(compute, HIRES_WIDTH / VULKAN_RASTER_THREADS,
						   target->bottom - target->top, 1);
	SDL_EndGPUComputePass(compute);
	/* Ending the raster pass establishes the dependency before shadow reads. */
	if (view->shadow_active) {
		compute = SDL_BeginGPUComputePass(command, NULL, 0, &output, 1);
		if (compute == NULL) {
			SDL_CancelGPUCommandBuffer(command);
			disable_after_failure("shadow pass");
			return 0;
		}
		SDL_GPUBuffer *lighting[] = {static_buffer.gpu, frame_buffer.gpu};
		SDL_BindGPUComputePipeline(compute, shadow_pipeline);
		SDL_BindGPUComputeStorageBuffers(compute, 0, lighting, 2);
		SDL_DispatchGPUCompute(compute, (target->right - target->left + 31) / 32,
							   (target->bottom - target->top + 31) / 32, 1);
		SDL_EndGPUComputePass(compute);
	}
	copy = SDL_BeginGPUCopyPass(command);
	if (copy == NULL) {
		SDL_CancelGPUCommandBuffer(command);
		disable_after_failure("readback pass");
		return 0;
	}
	SDL_GPUBufferRegion source = {pixel_buffer, 0,
								  (legacy_u32)(HIRES_WIDTH * (target->bottom - target->top) *
											   sizeof(struct HIRES_RASTER_SAMPLE))};
	SDL_GPUTransferBufferLocation destination = {download, 0};
	SDL_DownloadFromGPUBuffer(copy, &source, &destination);
	SDL_EndGPUCopyPass(copy);
	SDL_GPUFence *fence = SDL_SubmitGPUCommandBufferAndAcquireFence(command);
	if (fence == NULL) {
		disable_after_failure("submission");
		return 0;
	}
	legacy_s32 completed = SDL_WaitForGPUFences(device, true, &fence, 1);
	SDL_ReleaseGPUFence(device, fence);
	if (!completed) {
		disable_after_failure("fence wait");
		return 0;
	}
	uploaded_static_revision = shadows.static_revision;
	const struct HIRES_RASTER_SAMPLE *samples = SDL_MapGPUTransferBuffer(device, download, false);
	if (samples == NULL) {
		disable_after_failure("readback mapping");
		return 0;
	}
	legacy_s32 imported = hires_raster_import(target, samples, view->shadow_active);
	SDL_UnmapGPUTransferBuffer(device, download);
	if (imported) {
		submissions++;
	}
	return imported;
}

#else

void render_vulkan_initialize(void)
{
}
void render_vulkan_shutdown(void)
{
}
legacy_s32 render_vulkan_available(void)
{
	return 0;
}
legacy_s32 render_vulkan_enabled(void)
{
	return 0;
}
void render_vulkan_set_enabled(legacy_s32 enabled)
{
	(void)enabled;
}
legacy_u32 render_vulkan_submission_count(void)
{
	return 0;
}
legacy_s32 render_vulkan_scene_begin(const struct HIRES_RASTER_TARGET *target)
{
	(void)target;
	return 0;
}
void render_vulkan_scene_span(legacy_s32 left, legacy_s32 right, legacy_s32 y, legacy_f64 inverse_z,
							  legacy_f64 depth_step, legacy_u32 family, legacy_s32 depth_mode,
							  legacy_u16 color, legacy_u16 alternate, legacy_u16 pattern,
							  legacy_s32 paint_mode, legacy_s32 depth_test)
{
	(void)left;
	(void)right;
	(void)y;
	(void)inverse_z;
	(void)depth_step;
	(void)family;
	(void)depth_mode;
	(void)color;
	(void)alternate;
	(void)pattern;
	(void)paint_mode;
	(void)depth_test;
}
legacy_s32 render_vulkan_scene_end(const struct RENDER_VULKAN_VIEW *view)
{
	(void)view;
	return 0;
}

#endif
