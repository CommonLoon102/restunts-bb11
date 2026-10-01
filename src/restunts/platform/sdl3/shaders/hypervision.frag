#version 450
#ifndef HV_DETAIL
#define HV_DETAIL 0
#endif
const uint PAINT_PATTERN = 1u;
const uint PAINT_ALTERNATE = 2u;
const uint FLAG_BACKGROUND = 1u;
const uint FLAG_DECAL = 2u;
const uint FLAG_GROUND_CLIP = 4u;
const uint BYTE_MASK = 255u;
const uint FLAG_SHIFT = 8u;
const uint PATTERN_WIDTH = 8u;
const float DEPTH_EPSILON = 4.76837158203125e-7;
const float DECAL_DEPTH_SCALE = 1.0001;
layout(location = 0) in float inverse_depth;
layout(location = 1) flat in uvec4 paint;
layout(location = 2) flat in uint flags;
layout(location = 3) flat in vec3 plane;
layout(location = 4) flat in uint use_plane;
#if HV_DETAIL
layout(set = 2, binding = 0) uniform usampler2D support_family;
layout(set = 2, binding = 1) uniform sampler2D support_depth;
#endif
layout(set = 3, binding = 0) uniform Frame { vec4 ground; vec4 config; } frame;
layout(location = 0) out uvec4 color_coverage;
layout(location = 1) out uint family;
layout(location = 2) out float physical_depth;
void main()
{
	uvec2 pixel = uvec2(gl_FragCoord.xy);
	uint mode = flags & BYTE_MASK;
	uint material_flags = flags >> FLAG_SHIFT;
	uint color = paint.y;
	uint bit = ((pixel.y & 1u) == 0u ? PATTERN_WIDTH : 0u) +
		PATTERN_WIDTH - 1u - (pixel.x & (PATTERN_WIDTH - 1u));
	bool pattern_set = (paint.w & (1u << bit)) != 0u;
	if (mode == PAINT_PATTERN && !pattern_set) {
		discard;
	}
	if (mode == PAINT_ALTERNATE && pattern_set) {
		color = paint.z;
	}
	float depth = use_plane != 0u ? dot(plane, vec3(gl_FragCoord.xy, 1.0)) : inverse_depth;
	family = paint.x;
	float comparison_depth = depth;
	if ((material_flags & FLAG_BACKGROUND) != 0u) {
		depth = comparison_depth = 0.0;
		family = 0u;
	} else {
		if (!(depth > 0.0)) {
			discard;
		}
#if HV_DETAIL
		uint owner = texelFetch(support_family, ivec2(pixel), 0).r;
		float supported_depth = texelFetch(support_depth, ivec2(pixel), 0).r;
		bool attached = (material_flags & FLAG_DECAL) != 0u;
		if (attached && owner != 0u && owner == family) {
			depth = supported_depth;
			comparison_depth = depth * DECAL_DEPTH_SCALE;
		} else {
			if (attached) {
				comparison_depth *= DECAL_DEPTH_SCALE;
			}
			if ((material_flags & FLAG_GROUND_CLIP) != 0u && owner == 0u &&
				frame.config.y != 0.0) {
				float ground_depth = dot(frame.ground.xyz, vec3(vec2(pixel), 1.0));
				if (comparison_depth + DEPTH_EPSILON * ground_depth < ground_depth) {
					discard;
				}
			}
			if (owner != 0u) {
				if (comparison_depth + DEPTH_EPSILON * supported_depth < supported_depth) {
					discard;
				}
				/* Coplanar material ties retain support precision instead of
				 * turning independently clipped edges into thin alternating stripes. */
				comparison_depth = max(comparison_depth, supported_depth);
			}
		}
#endif
	}
	physical_depth = depth;
	gl_FragDepth = clamp(comparison_depth * frame.config.x, 0.0, 1.0);
	color_coverage = uvec4(color & BYTE_MASK, 1u, 0u, 0u);
}
