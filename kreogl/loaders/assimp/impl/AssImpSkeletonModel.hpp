#pragma once

// kreogl
#include "kreogl/animation/SkeletonModel.hpp"

// glm
#include <glm/glm.hpp>

// AssImp
#include <assimp/scene.h>

namespace kreogl {
	struct KREOGL_EXPORT AssImpSkeletonModel : SkeletonModel {
		struct KREOGL_EXPORT Mesh {
			struct KREOGL_EXPORT Bone {
				const aiNode * node = nullptr;
				glm::mat4 offset{ 1.f };
			};
			std::vector<Bone> bones;
		};

		const aiNode * rootNode = nullptr;
		std::vector<Mesh> assimpMeshes;
	};
}