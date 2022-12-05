#include "position_proj_view_model_glsl.hpp"

namespace kreogl {
	const char * position_proj_view_model_glsl::vert = R"(
#version 330

layout (location = 0) in vec3 position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = proj * view * model * vec4(position, 1.0);
})";
}