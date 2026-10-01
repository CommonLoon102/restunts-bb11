#version 450
layout(location = 0) in vec3 position;
layout(location = 1) in uvec4 material;
layout(location = 2) in uint mode_flags;
layout(location = 3) in vec3 depth_plane;
layout(location = 4) in uint planar;
layout(set = 1, binding = 0) uniform View { vec4 extent; } view;
layout(location = 0) out float inverse_depth;
layout(location = 1) flat out uvec4 paint;
layout(location = 2) flat out uint flags;
layout(location = 3) flat out vec3 plane;
layout(location = 4) flat out uint use_plane;
void main()
{
	gl_Position = vec4(position.x * (2.0 / view.extent.x) - 1.0,
		1.0 - position.y * (2.0 / view.extent.y),
		clamp(position.z * view.extent.z, 0.0, 1.0), 1.0);
	inverse_depth = position.z;
	paint = material;
	flags = mode_flags;
	plane = depth_plane;
	use_plane = planar;
}
