#include "ColorGLSL.hpp"

namespace kreogl {
	const char * ColorGLSL::frag = R"(
#version 330

uniform vec4 color;
out vec4 outColor;

void main() {
    outColor = color;
})";
}