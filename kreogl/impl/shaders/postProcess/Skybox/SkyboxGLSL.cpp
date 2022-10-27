#include "SkyboxGLSL.hpp"

namespace kreogl {
	const char * SkyboxGLSL::vert = R"(
#version 330 core
layout (location = 0) in vec3 pos;

out vec3 TexCoords;

uniform mat4 proj;
uniform mat4 view;

void main() {
    TexCoords = pos;
    vec4 projectedPos = proj * view * vec4(pos, 1.0);
    gl_Position = projectedPos.xyww;
})";

	const char * SkyboxGLSL::frag = R"(
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube tex;
uniform vec4 color;

void main() {
    FragColor = texture(tex, TexCoords) * color;
})";
}