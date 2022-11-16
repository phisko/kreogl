#pragma once

// kreogl
#include "kreogl/impl/shaders/Uniform.hpp"

#ifndef KREOGL_SKELETON_MAX_BONES
#	define KREOGL_SKELETON_MAX_BONES 128
#endif

namespace kreogl {
	struct KREOGL_EXPORT SkeletalGLSL {
		static const char * vert;
		Uniform<glm::mat4> model = { "model" };
		Uniform<glm::mat4> view = { "view" };
		Uniform<glm::mat4> proj = { "proj" };
		Uniform<glm::mat4> bones = { "bones" }; // [KREOGL_SKELETON_MAX_BONES] but we'll only use the first and do a single upload

		static const char * frag;
		Uniform<bool> hasTexture = { "hasTexture" };
		Uniform<size_t> texture_diffuse = { "texture_diffuse" };
		Uniform<size_t> texture_specular = { "texture_specular" };
		Uniform<glm::vec4> diffuseColor = { "diffuseColor" };
		Uniform<glm::vec4> specularColor = { "specularColor" };
		Uniform<glm::vec4> color = { "color" };
		Uniform<glm::vec4> userData = { "userData" };
	};
}