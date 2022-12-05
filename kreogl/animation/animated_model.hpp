#pragma once

// stl
#include <memory>

// glm
#include <glm/glm.hpp>

// kreogl
#include "kreogl/model/model.hpp"
#include "kreogl/impl/texture/image_texture.hpp"
#include "animation_model.hpp"
#include "skeleton_model.hpp"

#ifndef KREOGL_BONE_INFO_PER_VERTEX
#	define KREOGL_BONE_INFO_PER_VERTEX 4
#endif

namespace kreogl {
	class KREOGL_EXPORT animated_model : public model {
	public:
		~animated_model() noexcept override = default;
		using model::model;

	public:
		struct vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 tex_coords;
			float bone_weights[KREOGL_BONE_INFO_PER_VERTEX] = { 0.f };
			unsigned int bone_ids[KREOGL_BONE_INFO_PER_VERTEX] = { std::numeric_limits<unsigned int>::max() };
		};

		struct mesh_textures {
			std::vector<kreogl::image_texture> diffuse_textures;
			glm::vec4 diffuse_color;

			std::vector<kreogl::image_texture> specular_textures;
			glm::vec4 specular_color;
		};

	public:
		std::unique_ptr<skeleton_model> skeleton = nullptr;
		std::vector<mesh_textures> textures;
		std::vector<std::unique_ptr<animation_model>> animations;
	};
}