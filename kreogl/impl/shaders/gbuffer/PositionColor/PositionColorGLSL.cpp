#include "PositionColorGLSL.hpp"

namespace kreogl {
    const char * PositionColorGLSL::vert = R"(
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 viewPos;

out vec4 WorldPosition;
out vec3 EyeRelativePos;
out vec3 Color;

void main() {
	WorldPosition = model * vec4(position, 1.0);
	EyeRelativePos = WorldPosition.xyz - viewPos;
	Color = color;

	gl_Position = proj * view * WorldPosition;
})";

    const char * PositionColorGLSL::frag = R"(
#version 330

in vec4 WorldPosition;
in vec3 EyeRelativePos;
in vec3 Color;

uniform vec4 color;
uniform vec4 userData;

layout (location = 0) out vec4 gposition;
layout (location = 1) out vec4 gnormal;
layout (location = 2) out vec4 gdiffuse;
layout (location = 3) out vec4 gspecular;
layout (location = 4) out vec4 guserData;

void applyTransparency(float a);

void main() {
	applyTransparency(color.a);

    gposition = WorldPosition;
    gnormal = vec4(-normalize(cross(dFdy(EyeRelativePos), dFdx(EyeRelativePos))), 1.0);
    gdiffuse = vec4(Color * color.rgb, 0.0); // 4th component is ShouldIgnoreLighting
    gspecular = vec4(Color * color.rgb, 1.0);
    guserData = userData;
})";
}