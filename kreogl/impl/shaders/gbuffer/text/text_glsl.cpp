#include "text_glsl.hpp"

namespace kreogl {
	const char * text_glsl::vert = R"(
#version 330 core

layout (location = 0) in vec4 vertex; // vec2 pos, vec2 tex

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 view_pos;

out vec4 WorldPosition;
out vec3 EyeRelativePos;
out vec2 TexCoords;

void main() {
	WorldPosition = model * vec4(vertex.xy, 0.0, 1.0);
	EyeRelativePos = WorldPosition.xyz - view_pos;
    TexCoords = vertex.zw;

    gl_Position = proj * view * WorldPosition;
})";

	const char * text_glsl::frag = R"(
#version 330 core

uniform sampler2D tex;
uniform vec4 color;
uniform vec4 user_data;

in vec4 WorldPosition;
in vec3 EyeRelativePos;
in vec2 TexCoords;

layout (location = 0) out vec4 gposition;
layout (location = 1) out vec3 gnormal;
layout (location = 2) out vec4 gdiffuse;
layout (location = 3) out vec4 gspecular;
layout (location = 4) out vec4 guser_data;

void apply_transparency(float alpha);

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(tex, TexCoords).r);
	sampled *= color;

	apply_transparency(sampled.a);

    gposition = WorldPosition;
    gnormal = -normalize(cross(dFdy(EyeRelativePos), dFdx(EyeRelativePos)));
    gdiffuse = vec4((sampled).xyz, 1.0); // don't apply lighting
	gspecular = vec4(0.0);
	guser_data = user_data;
})";
}