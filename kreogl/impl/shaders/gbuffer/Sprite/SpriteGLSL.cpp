#include "SpriteGLSL.hpp"

namespace kreogl {
    const char * SpriteGLSL::vert = R"(
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 viewPos;

out vec4 WorldPosition;
out vec3 EyeRelativePos;
out vec2 TexCoords;

void main() {
	WorldPosition = model * vec4(position, 1.0);
	EyeRelativePos = WorldPosition.xyz - viewPos;
	TexCoords = texCoords;

	gl_Position = proj * view * WorldPosition;
})";

    const char * SpriteGLSL::frag = R"(
#version 330

uniform sampler2D tex;
uniform vec4 color;
uniform vec4 userData;

in vec4 WorldPosition;
in vec3 EyeRelativePos;
in vec2 TexCoords;

layout (location = 0) out vec4 gposition;
layout (location = 1) out vec3 gnormal;
layout (location = 2) out vec4 gdiffuse;
layout (location = 3) out vec4 gspecular;
layout (location = 4) out vec4 guserData;

void applyTransparency(float alpha);

void main() {
	vec4 pix = texture(tex, TexCoords);
	pix *= color;

	applyTransparency(pix.a);

    gposition = WorldPosition;
    gnormal = -normalize(cross(dFdy(EyeRelativePos), dFdx(EyeRelativePos)));
	gdiffuse = vec4(pix.rgb, 1.0); // don't apply lighting
	gspecular = vec4(0.0); // don't apply lighting
	guserData = userData;
})";
}