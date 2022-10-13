#include "DebugGLSL.hpp"

namespace kreogl {
    // Copied from PositionColorGLSL::vert
    // Removed the `color` vertex attribute and output
    const char * DebugGLSL::vert = R"(
#version 330

layout (location = 0) in vec3 position;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 viewPos;

out vec4 WorldPosition;
out vec3 EyeRelativePos;

void main() {
	WorldPosition = model * vec4(position, 1.0);
	EyeRelativePos = WorldPosition.xyz - viewPos;

	gl_Position = proj * view * WorldPosition;
})";

    // Copied from PositionColorGLSL::frag
    // Removed the `color` input
    // Set ShouldIgnoreLighting to 1
    // Set specular to 0
    const char * DebugGLSL::frag = R"(
#version 330

in vec4 WorldPosition;
in vec3 EyeRelativePos;

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
    gdiffuse = vec4(color.rgb, 1.0); // 4th component is ShouldIgnoreLighting
    gspecular = vec4(0.0);
    guserData = userData;
})";
}
