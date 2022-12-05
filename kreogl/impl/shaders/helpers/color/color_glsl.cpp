#include "color_glsl.hpp"

namespace kreogl {
	const char * color_glsl::frag = R"(
#version 330

uniform vec4 color;
out vec4 out_color;

void main() {
    out_color = color;
})";
}