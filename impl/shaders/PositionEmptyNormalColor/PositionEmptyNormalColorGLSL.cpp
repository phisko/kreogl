#include "PositionEmptyNormalColorGLSL.hpp"

namespace kreogl {
    const char * PositionEmptyNormalColorGLSL::vert = R"(
#version 330

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 color;

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
	//Color = vec3(1.0); // This is pretty

	gl_Position = proj * view * WorldPosition;
})";

    const char * PositionEmptyNormalColorGLSL::frag = R"(
#version 330

in vec4 WorldPosition;
in vec3 EyeRelativePos;
in vec3 Color;

uniform vec4 color;
uniform vec4 userData;

layout (location = 0) out vec4 gposition;
layout (location = 1) out vec3 gnormal;
layout (location = 2) out vec4 gdiffuse;
layout (location = 3) out vec4 gspecular;
layout (location = 4) out vec4 guserData;

void applyTransparency(float a);

void main() {
	applyTransparency(color.a);

    gposition = WorldPosition;
    gnormal = -normalize(cross(dFdy(EyeRelativePos), dFdx(EyeRelativePos)));
    gdiffuse = vec4(Color * color.rgb, 0.0);
    gspecular = vec4(Color * color.rgb, 0.0);
    guserData = userData;
})";
}