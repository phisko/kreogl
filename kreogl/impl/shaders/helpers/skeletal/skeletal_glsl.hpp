#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"

#ifndef KREOGL_SKELETON_MAX_BONES
#define KREOGL_SKELETON_MAX_BONES 128
#endif

namespace kreogl {
	struct KREOGL_EXPORT skeletal_glsl {
		static const char * vert;
		uniform<glm::mat4> model = { "model" };
		uniform<glm::mat4> view = { "view" };
		uniform<glm::mat4> proj = { "proj" };
		uniform<glm::mat4> bones = { "bones" }; // [KREOGL_SKELETON_MAX_BONES] but we'll only use the first and do a single upload

		static const char * frag;
		uniform<bool> has_diffuse_texture = { "has_diffuse_texture" };
		uniform<size_t> texture_diffuse = { "texture_diffuse" };
		uniform<bool> has_specular_texture = { "has_specular_texture" };
		uniform<size_t> texture_specular = { "texture_specular" };
		uniform<glm::vec4> diffuse_color = { "diffuse_color" };
		uniform<glm::vec4> specular_color = { "specular_color" };
		uniform<glm::vec4> color = { "color" };
		uniform<glm::vec4> user_data = { "user_data" };
	};
}