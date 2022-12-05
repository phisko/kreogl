#include "shadow_cube_glsl.hpp"

namespace kreogl {
	const char * shadow_cube_glsl::geom = R"(
#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 light_space_matrices[6];
out vec4 FragPos;

void main() {
    for (int face = 0; face < 6; ++face) {
        gl_Layer = face;
        for (int i = 0; i < 3; ++i) {
            FragPos = gl_in[i].gl_Position;
            gl_Position = light_space_matrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
)";

	const char * shadow_cube_glsl::frag = R"(
#version 330

in vec4 FragPos;

uniform vec3 light_pos;
uniform float far_plane;

void main() {
    float light_distance = length(FragPos.xyz - light_pos);
    light_distance /= far_plane;
    gl_FragDepth = light_distance;
}
)";
}