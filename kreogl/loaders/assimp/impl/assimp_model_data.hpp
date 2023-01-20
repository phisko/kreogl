#pragma once

// stl
#include <vector>

// glm
#include <glm/glm.hpp>

// kreogl
#include "kreogl/animation/animated_model.hpp"
#include "kreogl/impl/default_constructors.hpp"
#include "kreogl/impl/texture/texture_data.hpp"
#include "assimp_animation_file.hpp"
#include "assimp_skeleton_model.hpp"

namespace kreogl {
	struct KREOGL_EXPORT assimp_mesh_data {
		KREOGL_MOVE_ONLY(assimp_mesh_data);

		std::vector<animated_model::vertex> vertices;
		std::vector<unsigned int> indices;

		std::vector<texture_data> diffuse_textures;
		glm::vec4 diffuse_color = { 0.f, 0.f, 0.f, 0.f };

		std::vector<texture_data> specular_textures;
		glm::vec4 specular_color = { 0.f, 0.f, 0.f, 0.f };
	};

	struct KREOGL_EXPORT assimp_model_data {
		KREOGL_MOVE_ONLY(assimp_model_data);

		std::vector<assimp_mesh_data> meshes;
		std::unique_ptr<assimp_animation_file> animations;
		std::unique_ptr<assimp_skeleton_model> skeleton;
	};
}