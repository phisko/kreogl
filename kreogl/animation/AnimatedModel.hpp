#pragma once

// stl
#include <memory>

// glm
#include <glm/glm.hpp>

// kreogl
#include "kreogl/model/Model.hpp"
#include "kreogl/impl/texture/ImageTexture.hpp"
#include "AnimationModel.hpp"
#include "SkeletonModel.hpp"

#ifndef KREOGL_BONE_INFO_PER_VERTEX
#	define KREOGL_BONE_INFO_PER_VERTEX 4
#endif

namespace kreogl {
	class KREOGL_EXPORT AnimatedModel : public Model {
		public:
		virtual ~AnimatedModel() noexcept = default;
		using Model::Model;

		public:
		struct Vertex {
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 texCoords;
			float boneWeights[KREOGL_BONE_INFO_PER_VERTEX] = { 0.f };
			unsigned int boneIDs[KREOGL_BONE_INFO_PER_VERTEX] = { std::numeric_limits<unsigned int>::max() };
		};

		struct MeshTextures {
			std::vector<kreogl::ImageTexture> diffuseTextures;
			glm::vec4 diffuseColor;

			std::vector<kreogl::ImageTexture> specularTextures;
			glm::vec4 specularColor;
		};

		public:
		std::unique_ptr<SkeletonModel> skeleton = nullptr;
		std::vector<MeshTextures> meshTextures;
		std::vector<std::unique_ptr<AnimationModel>> animations;
	};
}