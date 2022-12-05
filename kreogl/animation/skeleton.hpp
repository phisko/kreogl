#pragma once

// stl
#include <vector>

// glm
#include <glm/glm.hpp>

namespace kreogl {
	struct KREOGL_EXPORT skeleton {
		struct KREOGL_EXPORT mesh {
			// Indexed by skeleton_model::mesh::bone_names
			std::vector<glm::mat4> bone_mats_bone_space;
			std::vector<glm::mat4> bone_mats_mesh_space;
		};

		std::vector<mesh> meshes;
	};
}