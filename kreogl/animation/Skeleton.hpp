#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace kreogl {
	struct KREOGL_EXPORT Skeleton {
		struct KREOGL_EXPORT Mesh {
			// Indexed by SkeletonModel::Mesh::boneNames
			std::vector<glm::mat4> boneMatsBoneSpace;
			std::vector<glm::mat4> boneMatsMeshSpace;
		};

		std::vector<Mesh> meshes;
	};
}