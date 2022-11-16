#pragma once

// glm
#include <glm/glm.hpp>

// AssImp
#include <assimp/scene.h>

// kreogl
#include "kreogl/animation/SkeletonModel.hpp"

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