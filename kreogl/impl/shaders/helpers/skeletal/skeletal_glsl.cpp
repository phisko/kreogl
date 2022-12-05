#include "skeletal_glsl.hpp"

#define MACRO_AS_STRING_IMPL(macro) #macro
#define MACRO_AS_STRING(macro) MACRO_AS_STRING_IMPL(macro)

namespace kreogl {
	const char * skeletal_glsl::vert = R"(
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coords;
layout (location = 3) in vec4 bone_weights;
layout (location = 4) in ivec4 bone_ids;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = )" MACRO_AS_STRING(KREOGL_SKELETON_MAX_BONES) R"(;
uniform mat4 bones[MAX_BONES];

out vec4 WorldPosition;
out vec3 Normal;
out vec2 TexCoords;

void main() {
	mat4 boneMatrix = bones[bone_ids[0]] * bone_weights[0];
	boneMatrix += bones[bone_ids[1]] * bone_weights[1];
	boneMatrix += bones[bone_ids[2]] * bone_weights[2];
	boneMatrix += bones[bone_ids[3]] * bone_weights[3];

	WorldPosition = model * boneMatrix * vec4(position, 1.0);
	Normal = (boneMatrix * vec4(normal, 0.0)).xyz;
	TexCoords = tex_coords;

	gl_Position = proj * view * WorldPosition;
})";

	const char * skeletal_glsl::frag = R"(
#version 330

in vec4 WorldPosition;
in vec3 Normal;
in vec2 TexCoords;

layout (location = 0) out vec4 gposition;
layout (location = 1) out vec3 gnormal;
layout (location = 2) out vec4 gdiffuse;
layout (location = 3) out vec4 gspecular;
layout (location = 4) out vec4 guser_data;

uniform int has_texture;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform vec4 diffuse_color;
uniform vec4 specular_color;

uniform vec4 user_data;
uniform vec4 color;

void apply_transparency(float a);

void main() {
	vec4 total_diffuse;
	vec4 total_specular;
	if (has_texture == 0) {
		total_diffuse = diffuse_color;
		total_specular = specular_color;
	}
	else {
		total_diffuse = texture(texture_diffuse, TexCoords);
		total_specular = texture(texture_specular, TexCoords);
	}

	total_diffuse *= color;
	total_specular *= color;

	// apply_transparency(total_diffuse.a);

	gposition = WorldPosition;
	gnormal = Normal;
	guser_data = user_data;
	gdiffuse = vec4(total_diffuse.xyz, 0.0);
	gspecular = vec4(total_specular.xyz, 0.0);
})";
}