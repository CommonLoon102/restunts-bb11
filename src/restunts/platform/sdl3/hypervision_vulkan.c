#include "hypervision_vulkan.h"
#include "hypervision_vulkan_shaders.h"
#include <SDL3/SDL.h>
#include <float.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define HV_VK_INITIAL_VERTICES 4096U
#define HV_VK_TRIANGLE_VERTICES 3U
#define HV_VK_ATTRIBUTE_COUNT 5U
#define HV_VK_ATTACHMENT_COUNT 3U
#define HV_VK_SNAPSHOT_COUNT 2U
#define HV_VK_PIXEL_BYTES LEGACY_DWORD_BYTES
#define HV_VK_MAX_PIXELS ((legacy_u32)HIRES_WIDTH * HIRES_HEIGHT)
#define HV_VK_DOWNLOAD_BYTES (HV_VK_MAX_PIXELS * HV_VK_PIXEL_BYTES * HV_VK_ATTACHMENT_COUNT)
#define HV_VK_GEOMETRY_EPSILON 1e-9
#define HV_VK_DEPTH_EPSILON (4.0 * FLT_EPSILON)
#define HV_VK_MODE_SHIFT LEGACY_BYTE_BITS
#define HV_VK_BAND_LIMIT (HYPERVISION_MAX_VERTICES * (HYPERVISION_MAX_VERTICES + 1U) / 2U)
#define HV_VK_QUAD_VERTICES 4U
#define HV_VK_QUAD_TRIANGLES 2U

enum HV_VK_ATTACHMENT { HV_VK_COLOR, HV_VK_FAMILY, HV_VK_INVERSE_DEPTH };
enum HV_VK_PHASE { HV_VK_BACKGROUND, HV_VK_SURFACE, HV_VK_DETAIL, HV_VK_CUTOUT, HV_VK_PHASES };

struct HV_VK_VERTEX {
	legacy_f32 x, y, inverse_z;
	legacy_u32 family, color, alternate, pattern, mode_flags;
	legacy_f32 plane[3];
	legacy_u32 planar;
};
struct HV_VK_RANGE {
	legacy_u32 first, count;
};
struct HV_VK_FRAME_UNIFORM {
	legacy_f32 ground[4], config[4];
};
struct HV_VK_STATE {
	SDL_GPUDevice *device;
	SDL_GPUGraphicsPipeline *surface_pipeline, *detail_pipeline;
	SDL_GPUSampler *sampler;
	SDL_GPUTexture *attachments[HV_VK_ATTACHMENT_COUNT];
	SDL_GPUTexture *snapshots[HV_VK_SNAPSHOT_COUNT];
	SDL_GPUTexture *depth;
	SDL_GPUTextureFormat depth_format;
	SDL_GPUBuffer *vertex_buffer;
	SDL_GPUTransferBuffer *upload, *download;
	void *mapped_download;
	struct HV_VK_VERTEX *vertices;
	legacy_u32 vertex_count, vertex_capacity, buffer_capacity;
	legacy_s32 hardware;
};
static struct HV_VK_STATE state;
static const SDL_GPUTextureFormat attachment_formats[HV_VK_ATTACHMENT_COUNT] = {
	SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UINT, SDL_GPU_TEXTUREFORMAT_R32_UINT,
	SDL_GPU_TEXTUREFORMAT_R32_FLOAT};

static legacy_f64 hv_vk_abs(legacy_f64 value)
{
	return value < 0 ? -value : value;
}

static void hv_vk_release(struct HV_VK_STATE *context)
{
	if (context->device != NULL) {
		if (context->mapped_download != NULL) {
			SDL_UnmapGPUTransferBuffer(context->device, context->download);
		}
		if (context->surface_pipeline != NULL) {
			SDL_ReleaseGPUGraphicsPipeline(context->device, context->surface_pipeline);
		}
		if (context->detail_pipeline != NULL) {
			SDL_ReleaseGPUGraphicsPipeline(context->device, context->detail_pipeline);
		}
		if (context->sampler != NULL) {
			SDL_ReleaseGPUSampler(context->device, context->sampler);
		}
		for (legacy_u32 index = 0; index < HV_VK_ATTACHMENT_COUNT; index++) {
			if (context->attachments[index] != NULL) {
				SDL_ReleaseGPUTexture(context->device, context->attachments[index]);
			}
		}
		for (legacy_u32 index = 0; index < HV_VK_SNAPSHOT_COUNT; index++) {
			if (context->snapshots[index] != NULL) {
				SDL_ReleaseGPUTexture(context->device, context->snapshots[index]);
			}
		}
		if (context->depth != NULL) {
			SDL_ReleaseGPUTexture(context->device, context->depth);
		}
		if (context->vertex_buffer != NULL) {
			SDL_ReleaseGPUBuffer(context->device, context->vertex_buffer);
		}
		if (context->upload != NULL) {
			SDL_ReleaseGPUTransferBuffer(context->device, context->upload);
		}
		if (context->download != NULL) {
			SDL_ReleaseGPUTransferBuffer(context->device, context->download);
		}
		SDL_DestroyGPUDevice(context->device);
	}
	free(context->vertices);
	memset(context, 0, sizeof(*context));
}

void hv_vulkan_shutdown(void)
{
	hv_vk_release(&state);
}

const legacy_char *hv_vulkan_name(void)
{
	return state.device != NULL
			   ? SDL_GetStringProperty(SDL_GetGPUDeviceProperties(state.device),
									   SDL_PROP_GPU_DEVICE_NAME_STRING, "Unknown Vulkan device")
			   : "Unavailable";
}

const legacy_char *hv_vulkan_driver(void)
{
	return state.device != NULL
			   ? SDL_GetStringProperty(SDL_GetGPUDeviceProperties(state.device),
									   SDL_PROP_GPU_DEVICE_DRIVER_INFO_STRING, "Vulkan")
			   : "Unavailable";
}

legacy_s32 hv_vulkan_hardware_accelerated(void)
{
	return state.device != NULL && state.hardware;
}

static SDL_GPUTexture *hv_vk_texture(struct HV_VK_STATE *context, SDL_GPUTextureFormat format,
									 SDL_GPUTextureUsageFlags usage)
{
	SDL_GPUTextureCreateInfo info = {0};
	info.type = SDL_GPU_TEXTURETYPE_2D;
	info.format = format;
	info.usage = usage;
	info.width = HIRES_WIDTH;
	info.height = HIRES_HEIGHT;
	info.layer_count_or_depth = 1;
	info.num_levels = 1;
	info.sample_count = SDL_GPU_SAMPLECOUNT_1;
	return SDL_CreateGPUTexture(context->device, &info);
}

static SDL_GPUShader *hv_vk_shader(struct HV_VK_STATE *context, const legacy_u32 *code, size_t size,
								   SDL_GPUShaderStage stage, legacy_u32 samplers)
{
	SDL_GPUShaderCreateInfo info = {0};
	info.code = (const legacy_u8 *)code;
	info.code_size = size;
	info.entrypoint = "main";
	info.format = SDL_GPU_SHADERFORMAT_SPIRV;
	info.stage = stage;
	info.num_samplers = samplers;
	info.num_uniform_buffers = 1;
	return SDL_CreateGPUShader(context->device, &info);
}

static legacy_s32 hv_vk_pipelines(struct HV_VK_STATE *context)
{
	SDL_GPUShader *vertex =
		hv_vk_shader(context, hv_vulkan_vertex_spirv, sizeof(hv_vulkan_vertex_spirv),
					 SDL_GPU_SHADERSTAGE_VERTEX, 0);
	SDL_GPUShader *surface =
		hv_vk_shader(context, hv_vulkan_surface_spirv, sizeof(hv_vulkan_surface_spirv),
					 SDL_GPU_SHADERSTAGE_FRAGMENT, 0);
	SDL_GPUShader *detail =
		hv_vk_shader(context, hv_vulkan_detail_spirv, sizeof(hv_vulkan_detail_spirv),
					 SDL_GPU_SHADERSTAGE_FRAGMENT, HV_VK_SNAPSHOT_COUNT);
	if (vertex != NULL && surface != NULL && detail != NULL) {
		SDL_GPUVertexBufferDescription buffer = {0};
		buffer.pitch = sizeof(struct HV_VK_VERTEX);
		buffer.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
		SDL_GPUVertexAttribute attributes[HV_VK_ATTRIBUTE_COUNT] = {
			{0, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(struct HV_VK_VERTEX, x)},
			{1, 0, SDL_GPU_VERTEXELEMENTFORMAT_UINT4, offsetof(struct HV_VK_VERTEX, family)},
			{2, 0, SDL_GPU_VERTEXELEMENTFORMAT_UINT, offsetof(struct HV_VK_VERTEX, mode_flags)},
			{3, 0, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, offsetof(struct HV_VK_VERTEX, plane)},
			{4, 0, SDL_GPU_VERTEXELEMENTFORMAT_UINT, offsetof(struct HV_VK_VERTEX, planar)}};
		SDL_GPUColorTargetDescription colors[HV_VK_ATTACHMENT_COUNT] = {0};
		for (legacy_u32 index = 0; index < HV_VK_ATTACHMENT_COUNT; index++) {
			colors[index].format = attachment_formats[index];
		}
		SDL_GPUGraphicsPipelineCreateInfo info = {0};
		info.vertex_shader = vertex;
		info.fragment_shader = surface;
		info.vertex_input_state.vertex_buffer_descriptions = &buffer;
		info.vertex_input_state.num_vertex_buffers = 1;
		info.vertex_input_state.vertex_attributes = attributes;
		info.vertex_input_state.num_vertex_attributes = HV_VK_ATTRIBUTE_COUNT;
		info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
		info.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
		info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
		info.rasterizer_state.enable_depth_clip = true;
		info.multisample_state.sample_count = SDL_GPU_SAMPLECOUNT_1;
		info.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_GREATER_OR_EQUAL;
		info.depth_stencil_state.enable_depth_test = true;
		info.depth_stencil_state.enable_depth_write = true;
		info.target_info.color_target_descriptions = colors;
		info.target_info.num_color_targets = HV_VK_ATTACHMENT_COUNT;
		info.target_info.depth_stencil_format = context->depth_format;
		info.target_info.has_depth_stencil_target = true;
		context->surface_pipeline = SDL_CreateGPUGraphicsPipeline(context->device, &info);
		info.fragment_shader = detail;
		context->detail_pipeline = SDL_CreateGPUGraphicsPipeline(context->device, &info);
	}
	if (vertex != NULL) {
		SDL_ReleaseGPUShader(context->device, vertex);
	}
	if (surface != NULL) {
		SDL_ReleaseGPUShader(context->device, surface);
	}
	if (detail != NULL) {
		SDL_ReleaseGPUShader(context->device, detail);
	}
	return context->surface_pipeline != NULL && context->detail_pipeline != NULL;
}

legacy_s32 hv_vulkan_initialize(void)
{
	if (state.device != NULL) {
		return 1;
	}
	struct HV_VK_STATE candidate = {0};
	SDL_PropertiesID properties = SDL_CreateProperties();
	if (properties == 0) {
		return 0;
	}
	const legacy_char *debug = SDL_getenv("RESTUNTS_VULKAN_DEBUG");
	SDL_SetStringProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_NAME_STRING, "vulkan");
	SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN, true);
	SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN,
						   debug != NULL && strcmp(debug, "1") == 0);
	SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_VERBOSE_BOOLEAN, false);
	SDL_SetBooleanProperty(
		properties, SDL_PROP_GPU_DEVICE_CREATE_VULKAN_REQUIRE_HARDWARE_ACCELERATION_BOOLEAN, true);
	candidate.device = SDL_CreateGPUDeviceWithProperties(properties);
	candidate.hardware = candidate.device != NULL;
	if (candidate.device == NULL) {
		/* F10 is an explicit comparison request, including software Vulkan in VMs. */
		SDL_SetBooleanProperty(
			properties, SDL_PROP_GPU_DEVICE_CREATE_VULKAN_REQUIRE_HARDWARE_ACCELERATION_BOOLEAN,
			false);
		candidate.device = SDL_CreateGPUDeviceWithProperties(properties);
	}
	SDL_DestroyProperties(properties);
	if (candidate.device == NULL) {
		return 0;
	}
	for (legacy_u32 index = 0; index < HV_VK_ATTACHMENT_COUNT; index++) {
		SDL_GPUTextureUsageFlags usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;
		if (index != HV_VK_COLOR) {
			usage |= SDL_GPU_TEXTUREUSAGE_SAMPLER;
		}
		if (!SDL_GPUTextureSupportsFormat(candidate.device, attachment_formats[index],
										  SDL_GPU_TEXTURETYPE_2D, usage)) {
			SDL_SetError("Vulkan lacks a required HyperVision metadata attachment format");
			goto failed;
		}
		candidate.attachments[index] = hv_vk_texture(&candidate, attachment_formats[index], usage);
		if (candidate.attachments[index] == NULL) {
			goto failed;
		}
		if (index != HV_VK_COLOR) {
			candidate.snapshots[index - 1U] =
				hv_vk_texture(&candidate, attachment_formats[index], SDL_GPU_TEXTUREUSAGE_SAMPLER);
			if (candidate.snapshots[index - 1U] == NULL) {
				goto failed;
			}
		}
	}
	candidate.depth_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
	if (!SDL_GPUTextureSupportsFormat(candidate.device, candidate.depth_format,
									  SDL_GPU_TEXTURETYPE_2D,
									  SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET)) {
		candidate.depth_format = SDL_GPU_TEXTUREFORMAT_D24_UNORM;
	}
	if (!SDL_GPUTextureSupportsFormat(candidate.device, candidate.depth_format,
									  SDL_GPU_TEXTURETYPE_2D,
									  SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET)) {
		SDL_SetError("Vulkan lacks a suitable HyperVision depth format");
		goto failed;
	}
	candidate.depth = hv_vk_texture(&candidate, candidate.depth_format,
									SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET);
	SDL_GPUTransferBufferCreateInfo transfer = {0};
	transfer.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
	transfer.size = HV_VK_DOWNLOAD_BYTES;
	candidate.download = SDL_CreateGPUTransferBuffer(candidate.device, &transfer);
	SDL_GPUSamplerCreateInfo sampler = {0};
	sampler.min_filter = sampler.mag_filter = SDL_GPU_FILTER_NEAREST;
	sampler.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
	sampler.address_mode_u = sampler.address_mode_v = sampler.address_mode_w =
		SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
	candidate.sampler = SDL_CreateGPUSampler(candidate.device, &sampler);
	if (candidate.depth == NULL || candidate.download == NULL || candidate.sampler == NULL ||
		!hv_vk_pipelines(&candidate)) {
		goto failed;
	}
	state = candidate;
	SDL_Log("HyperVision Vulkan: %s (%s; %s)", hv_vulkan_name(), hv_vulkan_driver(),
			state.hardware ? "hardware" : "software/other device");
	return 1;
failed:
	hv_vk_release(&candidate);
	return 0;
}

static legacy_s32 hv_vk_reserve_vertices(legacy_u32 needed)
{
	if (needed <= state.vertex_capacity) {
		return 1;
	}
	const legacy_u32 limit = LEGACY_U32_MAX / sizeof(*state.vertices);
	if (needed > limit) {
		return 0;
	}
	legacy_u32 capacity =
		state.vertex_capacity != 0 ? state.vertex_capacity : HV_VK_INITIAL_VERTICES;
	while (capacity < needed) {
		capacity = capacity > limit / 2U ? limit : capacity * 2U;
	}
	struct HV_VK_VERTEX *vertices = realloc(state.vertices, (size_t)capacity * sizeof(*vertices));
	if (vertices == NULL) {
		return 0;
	}
	state.vertices = vertices;
	state.vertex_capacity = capacity;
	return 1;
}

static legacy_f64 hv_vk_cross(const struct HYPERVISION_VERTEX *a,
							  const struct HYPERVISION_VERTEX *b,
							  const struct HYPERVISION_VERTEX *c)
{
	return (b->x - a->x) * (c->y - a->y) - (b->y - a->y) * (c->x - a->x);
}

static legacy_s32 hv_vk_on_segment(const struct HYPERVISION_VERTEX *a,
								   const struct HYPERVISION_VERTEX *b,
								   const struct HYPERVISION_VERTEX *point)
{
	return hv_vk_abs(hv_vk_cross(a, b, point)) <= HV_VK_GEOMETRY_EPSILON &&
		   point->x >= SDL_min(a->x, b->x) && point->x <= SDL_max(a->x, b->x) &&
		   point->y >= SDL_min(a->y, b->y) && point->y <= SDL_max(a->y, b->y);
}

static legacy_s32 hv_vk_edges_cross(const struct HYPERVISION_VERTEX *a,
									const struct HYPERVISION_VERTEX *b,
									const struct HYPERVISION_VERTEX *c,
									const struct HYPERVISION_VERTEX *d)
{
	legacy_f64 first = hv_vk_cross(a, b, c), second = hv_vk_cross(a, b, d);
	legacy_f64 third = hv_vk_cross(c, d, a), fourth = hv_vk_cross(c, d, b);
	return (((first > 0 && second < 0) || (first < 0 && second > 0)) &&
			((third > 0 && fourth < 0) || (third < 0 && fourth > 0))) ||
		   hv_vk_on_segment(a, b, c) || hv_vk_on_segment(a, b, d) || hv_vk_on_segment(c, d, a) ||
		   hv_vk_on_segment(c, d, b);
}

static legacy_u32 hv_vk_plane(const struct HYPERVISION_VERTEX *points, legacy_u32 count,
							  legacy_f32 plane[3])
{
	legacy_f64 largest = 0;
	legacy_u32 selected = 0;
	for (legacy_u32 index = 1; index + 1U < count; index++) {
		legacy_f64 area = hv_vk_abs(hv_vk_cross(points, &points[index], &points[index + 1U]));
		if (area > largest) {
			largest = area;
			selected = index;
		}
	}
	if (selected == 0) {
		return 0;
	}
	const struct HYPERVISION_VERTEX *a = &points[selected], *b = &points[selected + 1U];
	legacy_f64 ax = a->x - points->x, ay = a->y - points->y;
	legacy_f64 bx = b->x - points->x, by = b->y - points->y;
	legacy_f64 az = a->inverse_z - points->inverse_z, bz = b->inverse_z - points->inverse_z;
	legacy_f64 inverse_area = 1.0 / (ax * by - bx * ay);
	legacy_f64 x = (az * by - bz * ay) * inverse_area;
	legacy_f64 y = (ax * bz - bx * az) * inverse_area;
	legacy_f64 origin = points->inverse_z - points->x * x - points->y * y;
	if (!(x >= -FLT_MAX && x <= FLT_MAX && y >= -FLT_MAX && y <= FLT_MAX && origin >= -FLT_MAX &&
		  origin <= FLT_MAX)) {
		return 0;
	}
	for (legacy_u32 index = 0; index < count; index++) {
		legacy_f64 error =
			origin + points[index].x * x + points[index].y * y - points[index].inverse_z;
		if (hv_vk_abs(error) > points[index].inverse_z * HV_VK_DEPTH_EPSILON) {
			return 0;
		}
	}
	plane[0] = (legacy_f32)x;
	plane[1] = (legacy_f32)y;
	plane[2] = (legacy_f32)origin;
	return 1;
}

static void hv_vk_emit(const struct HYPERVISION_VERTEX *point,
					   const struct HYPERVISION_MATERIAL *material, const legacy_f32 *plane,
					   legacy_u32 planar)
{
	struct HV_VK_VERTEX *vertex = &state.vertices[state.vertex_count++];
	vertex->x = (legacy_f32)point->x;
	vertex->y = (legacy_f32)point->y;
	vertex->inverse_z = (legacy_f32)point->inverse_z;
	vertex->family = material->family;
	vertex->color = material->color;
	vertex->alternate = material->alternate;
	vertex->pattern = material->pattern;
	vertex->mode_flags = material->mode | ((legacy_u32)material->flags << HV_VK_MODE_SHIFT);
	memcpy(vertex->plane, plane, sizeof(vertex->plane));
	vertex->planar = planar;
}

static struct HYPERVISION_VERTEX hv_vk_edge_point(const struct HYPERVISION_VERTEX *first,
												  const struct HYPERVISION_VERTEX *second,
												  legacy_f64 y)
{
	legacy_f64 along = (y - first->y) / (second->y - first->y);
	return (struct HYPERVISION_VERTEX){first->x + along * (second->x - first->x), y,
									   first->inverse_z +
										   along * (second->inverse_z - first->inverse_z)};
}

/* Authored contours can touch or cross themselves after projection. Between
 * consecutive vertex/intersection heights, edge order is fixed: even-odd pairs
 * describe trapezoids which the GPU can rasterize directly. This is geometric
 * tessellation only; it does not generate CPU pixels or sort surfaces by depth. */
static legacy_s32 hv_vk_even_odd(const struct HYPERVISION_VERTEX *points, legacy_u32 count,
								 const struct HYPERVISION_MATERIAL *material)
{
	legacy_f64 bands[HV_VK_BAND_LIMIT];
	legacy_u32 band_count = count;
	for (legacy_u32 first = 0; first < count; first++) {
		bands[first] = points[first].y;
		const struct HYPERVISION_VERTEX *a = &points[first], *b = &points[(first + 1U) % count];
		legacy_f64 ab_x = b->x - a->x, ab_y = b->y - a->y;
		for (legacy_u32 second = first + 1U; second < count; second++) {
			const struct HYPERVISION_VERTEX *c = &points[second];
			const struct HYPERVISION_VERTEX *d = &points[(second + 1U) % count];
			legacy_f64 cd_x = d->x - c->x, cd_y = d->y - c->y;
			legacy_f64 denominator = ab_x * cd_y - ab_y * cd_x;
			if (denominator == 0) {
				continue;
			}
			legacy_f64 ca_x = c->x - a->x, ca_y = c->y - a->y;
			legacy_f64 along_ab = (ca_x * cd_y - ca_y * cd_x) / denominator;
			legacy_f64 along_cd = (ca_x * ab_y - ca_y * ab_x) / denominator;
			if (along_ab > 0 && along_ab < 1 && along_cd > 0 && along_cd < 1) {
				bands[band_count++] = a->y + along_ab * ab_y;
			}
		}
	}
	for (legacy_u32 index = 1; index < band_count; index++) {
		legacy_f64 value = bands[index];
		legacy_u32 position = index;
		while (position != 0 && bands[position - 1U] > value) {
			bands[position] = bands[position - 1U];
			position--;
		}
		bands[position] = value;
	}
	legacy_f32 plane[3] = {0};
	legacy_u32 planar = hv_vk_plane(points, count, plane);
	for (legacy_u32 band = 1; band < band_count; band++) {
		legacy_f64 top = bands[band - 1U], bottom = bands[band];
		legacy_f64 middle = (top + bottom) * 0.5;
		if (!(middle > top && middle < bottom)) {
			continue;
		}
		struct {
			legacy_f64 x;
			legacy_u32 edge;
		} crossings[HYPERVISION_MAX_VERTICES];
		legacy_u32 crossing_count = 0;
		for (legacy_u32 edge = 0; edge < count; edge++) {
			const struct HYPERVISION_VERTEX *a = &points[edge], *b = &points[(edge + 1U) % count];
			if ((a->y > middle) == (b->y > middle)) {
				continue;
			}
			legacy_f64 x = hv_vk_edge_point(a, b, middle).x;
			legacy_u32 position = crossing_count++;
			while (position != 0 && crossings[position - 1U].x > x) {
				crossings[position] = crossings[position - 1U];
				position--;
			}
			crossings[position].x = x;
			crossings[position].edge = edge;
		}
		for (legacy_u32 index = 0; index + 1U < crossing_count; index += 2U) {
			if (crossings[index + 1U].x - crossings[index].x <= HV_VK_GEOMETRY_EPSILON) {
				continue;
			}
			legacy_u32 left = crossings[index].edge, right = crossings[index + 1U].edge;
			struct HYPERVISION_VERTEX quad[HV_VK_QUAD_VERTICES] = {
				hv_vk_edge_point(&points[left], &points[(left + 1U) % count], top),
				hv_vk_edge_point(&points[right], &points[(right + 1U) % count], top),
				hv_vk_edge_point(&points[right], &points[(right + 1U) % count], bottom),
				hv_vk_edge_point(&points[left], &points[(left + 1U) % count], bottom)};
			const legacy_u32 triangles[HV_VK_TRIANGLE_VERTICES * HV_VK_QUAD_TRIANGLES] = {0, 1, 2,
																						  0, 2, 3};
			legacy_u32 needed = HV_VK_TRIANGLE_VERTICES * HV_VK_QUAD_TRIANGLES;
			if (needed > LEGACY_U32_MAX - state.vertex_count ||
				!hv_vk_reserve_vertices(state.vertex_count + needed)) {
				SDL_SetError("Cannot allocate HyperVision Vulkan contour geometry");
				return 0;
			}
			for (legacy_u32 vertex = 0; vertex < needed; vertex++) {
				hv_vk_emit(&quad[triangles[vertex]], material, plane, planar);
			}
		}
	}
	return 1;
}

/* Remove only zero-length edges and intermediate collinear vertices. Ear clipping
 * handles simple contours of either winding; crossing/touching contours use
 * even-odd geometric tessellation instead of an incorrect triangle fan. */
static legacy_s32 hv_vk_polygon(const struct HYPERVISION_POLYGON *polygon)
{
	struct HYPERVISION_VERTEX points[HYPERVISION_MAX_VERTICES];
	legacy_u32 count = 0;
	for (legacy_u32 index = 0; index < polygon->count; index++) {
		const struct HYPERVISION_VERTEX *point = &polygon->vertices[index];
		if (count == 0 || point->x != points[count - 1U].x || point->y != points[count - 1U].y) {
			points[count++] = *point;
		}
	}
	if (count > 1U && points[0].x == points[count - 1U].x && points[0].y == points[count - 1U].y) {
		count--;
	}
	for (legacy_u32 index = 0; count >= HV_VK_TRIANGLE_VERTICES && index < count;) {
		legacy_u32 previous = index == 0 ? count - 1U : index - 1U;
		legacy_u32 next = (index + 1U) % count;
		if (hv_vk_on_segment(&points[previous], &points[next], &points[index])) {
			memmove(&points[index], &points[index + 1U], (count - index - 1U) * sizeof(*points));
			count--;
			index = 0;
		} else {
			index++;
		}
	}
	if (count < HV_VK_TRIANGLE_VERTICES) {
		return 1;
	}
	for (legacy_u32 first = 0; first < count; first++) {
		legacy_u32 first_next = (first + 1U) % count;
		for (legacy_u32 second = first + 1U; second < count; second++) {
			legacy_u32 second_next = (second + 1U) % count;
			if (first_next != second && second_next != first &&
				hv_vk_edges_cross(&points[first], &points[first_next], &points[second],
								  &points[second_next])) {
				return hv_vk_even_odd(points, count, &polygon->material);
			}
		}
	}
	legacy_f64 area = 0;
	for (legacy_u32 index = 1; index + 1U < count; index++) {
		area += hv_vk_cross(points, &points[index], &points[index + 1U]);
	}
	if (hv_vk_abs(area) <= HV_VK_GEOMETRY_EPSILON) {
		return 1;
	}
	legacy_f64 winding = area < 0 ? -1 : 1;
	legacy_u32 needed = (count - 2U) * HV_VK_TRIANGLE_VERTICES;
	if (needed > LEGACY_U32_MAX - state.vertex_count ||
		!hv_vk_reserve_vertices(state.vertex_count + needed)) {
		SDL_SetError("Cannot allocate HyperVision Vulkan geometry");
		return 0;
	}
	legacy_f32 plane[3] = {0};
	legacy_u32 planar = hv_vk_plane(points, count, plane);
	legacy_u32 remaining[HYPERVISION_MAX_VERTICES];
	for (legacy_u32 index = 0; index < count; index++) {
		remaining[index] = index;
	}
	legacy_u32 contour_count = count, first_vertex = state.vertex_count;
	while (count > HV_VK_TRIANGLE_VERTICES) {
		legacy_s32 found = 0;
		for (legacy_u32 index = 0; index < count; index++) {
			legacy_u32 previous = index == 0 ? count - 1U : index - 1U;
			legacy_u32 next = (index + 1U) % count;
			const struct HYPERVISION_VERTEX *a = &points[remaining[previous]];
			const struct HYPERVISION_VERTEX *b = &points[remaining[index]];
			const struct HYPERVISION_VERTEX *c = &points[remaining[next]];
			if (hv_vk_cross(a, b, c) * winding <= HV_VK_GEOMETRY_EPSILON) {
				continue;
			}
			legacy_s32 contains = 0;
			for (legacy_u32 test = 0; test < count; test++) {
				if (test != previous && test != index && test != next) {
					const struct HYPERVISION_VERTEX *point = &points[remaining[test]];
					if (hv_vk_cross(a, b, point) * winding >= -HV_VK_GEOMETRY_EPSILON &&
						hv_vk_cross(b, c, point) * winding >= -HV_VK_GEOMETRY_EPSILON &&
						hv_vk_cross(c, a, point) * winding >= -HV_VK_GEOMETRY_EPSILON) {
						contains = 1;
						break;
					}
				}
			}
			if (contains) {
				continue;
			}
			hv_vk_emit(a, &polygon->material, plane, planar);
			hv_vk_emit(b, &polygon->material, plane, planar);
			hv_vk_emit(c, &polygon->material, plane, planar);
			memmove(&remaining[index], &remaining[index + 1U],
					(count - index - 1U) * sizeof(*remaining));
			count--;
			found = 1;
			break;
		}
		if (!found) {
			state.vertex_count = first_vertex;
			return hv_vk_even_odd(points, contour_count, &polygon->material);
		}
	}
	for (legacy_u32 index = 0; index < HV_VK_TRIANGLE_VERTICES; index++) {
		hv_vk_emit(&points[remaining[index]], &polygon->material, plane, planar);
	}
	return 1;
}

static enum HV_VK_PHASE hv_vk_phase(const struct HYPERVISION_MATERIAL *material)
{
	if ((material->flags & HYPERVISION_BACKGROUND) != 0U) {
		return HV_VK_BACKGROUND;
	}
	if (material->mode == HIRES_PAINT_PATTERN) {
		return HV_VK_CUTOUT;
	}
	return (material->flags & (HYPERVISION_DECAL | HYPERVISION_GROUND_CLIP)) != 0U ? HV_VK_DETAIL
																				   : HV_VK_SURFACE;
}

static legacy_s32 hv_vk_upload(SDL_GPUCommandBuffer *command)
{
	legacy_u32 bytes = state.vertex_count * sizeof(*state.vertices);
	if (bytes == 0) {
		return 1;
	}
	if (state.buffer_capacity < bytes) {
		SDL_GPUBufferCreateInfo buffer_info = {0};
		buffer_info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
		buffer_info.size = state.vertex_capacity * sizeof(*state.vertices);
		SDL_GPUTransferBufferCreateInfo transfer_info = {0};
		transfer_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
		transfer_info.size = buffer_info.size;
		SDL_GPUBuffer *buffer = SDL_CreateGPUBuffer(state.device, &buffer_info);
		SDL_GPUTransferBuffer *transfer = SDL_CreateGPUTransferBuffer(state.device, &transfer_info);
		if (buffer == NULL || transfer == NULL) {
			if (buffer != NULL) {
				SDL_ReleaseGPUBuffer(state.device, buffer);
			}
			if (transfer != NULL) {
				SDL_ReleaseGPUTransferBuffer(state.device, transfer);
			}
			return 0;
		}
		if (state.vertex_buffer != NULL) {
			SDL_ReleaseGPUBuffer(state.device, state.vertex_buffer);
			SDL_ReleaseGPUTransferBuffer(state.device, state.upload);
		}
		state.vertex_buffer = buffer;
		state.upload = transfer;
		state.buffer_capacity = buffer_info.size;
	}
	void *mapped = SDL_MapGPUTransferBuffer(state.device, state.upload, false);
	if (mapped == NULL) {
		return 0;
	}
	memcpy(mapped, state.vertices, bytes);
	SDL_UnmapGPUTransferBuffer(state.device, state.upload);
	SDL_GPUCopyPass *copy = SDL_BeginGPUCopyPass(command);
	if (copy == NULL) {
		return 0;
	}
	SDL_GPUTransferBufferLocation source = {state.upload, 0};
	SDL_GPUBufferRegion destination = {state.vertex_buffer, 0, bytes};
	SDL_UploadToGPUBuffer(copy, &source, &destination, false);
	SDL_EndGPUCopyPass(copy);
	return 1;
}

static legacy_s32 hv_vk_snapshot(SDL_GPUCommandBuffer *command, legacy_u32 width, legacy_u32 height)
{
	SDL_GPUCopyPass *copy = SDL_BeginGPUCopyPass(command);
	if (copy == NULL) {
		return 0;
	}
	for (legacy_u32 index = 0; index < HV_VK_SNAPSHOT_COUNT; index++) {
		SDL_GPUTextureLocation source = {0}, destination = {0};
		source.texture = state.attachments[index + 1U];
		destination.texture = state.snapshots[index];
		SDL_CopyGPUTextureToTexture(copy, &source, &destination, width, height, 1, false);
	}
	SDL_EndGPUCopyPass(copy);
	return 1;
}

static legacy_s32 hv_vk_draw(SDL_GPUCommandBuffer *command, const struct HV_VK_RANGE *range,
							 const struct HIRES_RASTER_TARGET *target, const SDL_Rect *clip,
							 const struct HV_VK_FRAME_UNIFORM *uniform, legacy_s32 detail,
							 legacy_s32 clear)
{
	SDL_GPUColorTargetInfo colors[HV_VK_ATTACHMENT_COUNT] = {0};
	for (legacy_u32 index = 0; index < HV_VK_ATTACHMENT_COUNT; index++) {
		colors[index].texture = state.attachments[index];
		colors[index].load_op = clear ? SDL_GPU_LOADOP_CLEAR : SDL_GPU_LOADOP_LOAD;
		colors[index].store_op = SDL_GPU_STOREOP_STORE;
	}
	SDL_GPUDepthStencilTargetInfo depth = {0};
	depth.texture = state.depth;
	depth.load_op = clear ? SDL_GPU_LOADOP_CLEAR : SDL_GPU_LOADOP_LOAD;
	depth.store_op = SDL_GPU_STOREOP_STORE;
	depth.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
	depth.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
	SDL_GPURenderPass *pass =
		SDL_BeginGPURenderPass(command, colors, HV_VK_ATTACHMENT_COUNT, &depth);
	if (pass == NULL) {
		return 0;
	}
	if (range->count != 0 && clip->w > 0 && clip->h > 0) {
		SDL_BindGPUGraphicsPipeline(pass, detail ? state.detail_pipeline : state.surface_pipeline);
		SDL_GPUViewport viewport = {0, 0, (legacy_f32)target->width, (legacy_f32)target->height,
									0, 1};
		SDL_SetGPUViewport(pass, &viewport);
		SDL_SetGPUScissor(pass, clip);
		SDL_GPUBufferBinding buffer = {state.vertex_buffer, 0};
		SDL_BindGPUVertexBuffers(pass, 0, &buffer, 1);
		legacy_f32 extent[4] = {(legacy_f32)target->width, (legacy_f32)target->height,
								uniform->config[0], 0};
		SDL_PushGPUVertexUniformData(command, 0, extent, sizeof(extent));
		SDL_PushGPUFragmentUniformData(command, 0, uniform, sizeof(*uniform));
		if (detail) {
			SDL_GPUTextureSamplerBinding bindings[HV_VK_SNAPSHOT_COUNT];
			for (legacy_u32 index = 0; index < HV_VK_SNAPSHOT_COUNT; index++) {
				bindings[index] =
					(SDL_GPUTextureSamplerBinding){state.snapshots[index], state.sampler};
			}
			SDL_BindGPUFragmentSamplers(pass, 0, bindings, HV_VK_SNAPSHOT_COUNT);
		}
		SDL_DrawGPUPrimitives(pass, range->count, 1, range->first, 0);
	}
	SDL_EndGPURenderPass(pass);
	return 1;
}

legacy_s32 hv_vulkan_render(const struct HYPERVISION_POLYGON *polygons, legacy_u32 count,
							const struct HIRES_RASTER_TARGET *target,
							const struct HIRES_DEPTH_PLANE *ground, legacy_s32 preserve_depth,
							struct HV_VULKAN_RESULT *result)
{
	if (state.device == NULL || target == NULL || result == NULL || preserve_depth ||
		(count != 0 && polygons == NULL) || target->width <= 0 || target->height <= 0 ||
		target->width > HIRES_WIDTH || target->height > HIRES_HEIGHT) {
		SDL_SetError("Unsupported HyperVision Vulkan batch");
		return 0;
	}
	legacy_f64 largest_depth = FLT_MIN;
	for (legacy_u32 index = 0; index < count; index++) {
		const struct HYPERVISION_POLYGON *polygon = &polygons[index];
		if (polygon->count > HYPERVISION_MAX_VERTICES ||
			polygon->material.mode > HIRES_PAINT_ALTERNATE) {
			SDL_SetError("Invalid HyperVision Vulkan polygon");
			return 0;
		}
		for (legacy_u32 vertex = 0; vertex < polygon->count; vertex++) {
			const struct HYPERVISION_VERTEX *point = &polygon->vertices[vertex];
			if (!(point->x >= -FLT_MAX && point->x <= FLT_MAX && point->y >= -FLT_MAX &&
				  point->y <= FLT_MAX && point->inverse_z >= FLT_MIN &&
				  point->inverse_z <= FLT_MAX)) {
				SDL_SetError("HyperVision Vulkan geometry exceeds float precision");
				return 0;
			}
			largest_depth = SDL_max(largest_depth, point->inverse_z);
		}
	}
	struct HV_VK_FRAME_UNIFORM uniform = {0};
	uniform.config[0] = (legacy_f32)(1.0 / largest_depth);
	if (!(uniform.config[0] >= FLT_MIN && uniform.config[0] <= FLT_MAX)) {
		SDL_SetError("HyperVision Vulkan depth range exceeds float precision");
		return 0;
	}
	if (ground != NULL) {
		if (!(ground->x_step >= -FLT_MAX && ground->x_step <= FLT_MAX &&
			  ground->y_step >= -FLT_MAX && ground->y_step <= FLT_MAX &&
			  ground->origin >= -FLT_MAX && ground->origin <= FLT_MAX)) {
			SDL_SetError("HyperVision Vulkan ground exceeds float precision");
			return 0;
		}
		uniform.ground[0] = (legacy_f32)ground->x_step;
		uniform.ground[1] = (legacy_f32)ground->y_step;
		uniform.ground[2] = (legacy_f32)ground->origin;
		uniform.config[1] = 1;
	}
	state.vertex_count = 0;
	struct HV_VK_RANGE ranges[HV_VK_PHASES] = {0};
	for (enum HV_VK_PHASE phase = HV_VK_BACKGROUND; phase < HV_VK_PHASES; phase++) {
		ranges[phase].first = state.vertex_count;
		for (legacy_u32 index = 0; index < count; index++) {
			if (hv_vk_phase(&polygons[index].material) == phase &&
				!hv_vk_polygon(&polygons[index])) {
				return 0;
			}
		}
		ranges[phase].count = state.vertex_count - ranges[phase].first;
	}
	if (state.mapped_download != NULL) {
		SDL_UnmapGPUTransferBuffer(state.device, state.download);
		state.mapped_download = NULL;
	}
	SDL_GPUCommandBuffer *command = SDL_AcquireGPUCommandBuffer(state.device);
	if (command == NULL) {
		return 0;
	}
	if (!hv_vk_upload(command)) {
		goto cancelled;
	}
	legacy_s32 left = SDL_clamp(SDL_max(target->left, target->depth_left), 0, target->width);
	legacy_s32 top = SDL_clamp(SDL_max(target->top, target->depth_top), 0, target->height);
	legacy_s32 right = SDL_clamp(SDL_min(target->right, target->depth_right), 0, target->width);
	legacy_s32 bottom = SDL_clamp(SDL_min(target->bottom, target->depth_bottom), 0, target->height);
	SDL_Rect clip = {left, top, SDL_max(0, right - left), SDL_max(0, bottom - top)};
	struct HV_VK_RANGE opaque = {ranges[HV_VK_BACKGROUND].first,
								 ranges[HV_VK_BACKGROUND].count + ranges[HV_VK_SURFACE].count};
	if (!hv_vk_draw(command, &opaque, target, &clip, &uniform, 0, 1)) {
		goto cancelled;
	}
	for (enum HV_VK_PHASE phase = HV_VK_DETAIL; phase <= HV_VK_CUTOUT; phase++) {
		if (ranges[phase].count != 0 &&
			(!hv_vk_snapshot(command, target->width, target->height) ||
			 !hv_vk_draw(command, &ranges[phase], target, &clip, &uniform, 1, 0))) {
			goto cancelled;
		}
	}
	SDL_GPUCopyPass *copy = SDL_BeginGPUCopyPass(command);
	if (copy == NULL) {
		goto cancelled;
	}
	legacy_u32 plane_bytes = (legacy_u32)target->width * target->height * HV_VK_PIXEL_BYTES;
	for (legacy_u32 index = 0; index < HV_VK_ATTACHMENT_COUNT; index++) {
		SDL_GPUTextureRegion source = {0};
		source.texture = state.attachments[index];
		source.w = target->width;
		source.h = target->height;
		source.d = 1;
		SDL_GPUTextureTransferInfo destination = {state.download, plane_bytes * index,
												  (legacy_u32)target->width,
												  (legacy_u32)target->height};
		SDL_DownloadFromGPUTexture(copy, &source, &destination);
	}
	SDL_EndGPUCopyPass(copy);
	SDL_GPUFence *fence = SDL_SubmitGPUCommandBufferAndAcquireFence(command);
	if (fence == NULL) {
		return 0;
	}
	legacy_s32 completed = SDL_WaitForGPUFences(state.device, true, &fence, 1);
	SDL_ReleaseGPUFence(state.device, fence);
	if (!completed) {
		return 0;
	}
	state.mapped_download = SDL_MapGPUTransferBuffer(state.device, state.download, false);
	if (state.mapped_download == NULL) {
		return 0;
	}
	const legacy_u8 *bytes = state.mapped_download;
	*result = (struct HV_VULKAN_RESULT){
		bytes, (const legacy_f32 *)(bytes + HV_VK_INVERSE_DEPTH * plane_bytes),
		(const legacy_u32 *)(bytes + HV_VK_FAMILY * plane_bytes), target->width, target->height};
	return 1;
cancelled:
	SDL_CancelGPUCommandBuffer(command);
	return 0;
}
