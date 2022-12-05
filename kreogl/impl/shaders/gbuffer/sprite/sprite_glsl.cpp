#include "sprite_glsl.hpp"

namespace kreogl {
	const char * sprite_glsl::vert = R"(
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform vec3 view_pos;

out vec4 WorldPosition;
out vec3 EyeRelativePos;
out vec2 TexCoords;

void main() {
	WorldPosition = model * vec4(position, 1.0);
	EyeRelativePos = WorldPosition.xyz - view_pos;
	TexCoords = tex_coords;

	gl_Position = proj * view * WorldPosition;
})";

	const char * sprite_glsl::frag = R"(
#version 330

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
	vec4 pix = texture(tex, TexCoords);
	pix *= color;

	apply_transparency(pix.a);

    gposition = WorldPosition;
    gnormal = -normalize(cross(dFdy(EyeRelativePos), dFdx(EyeRelativePos)));
	gdiffuse = vec4(pix.rgb, 1.0); // don't apply lighting
	gspecular = vec4(0.0); // don't apply lighting
	guser_data = user_data;
})";
}