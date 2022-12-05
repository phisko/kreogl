#pragma once

// glm
#include <glm/glm.hpp>

// assimp
#include <assimp/scene.h>

// kreogl
#include "kreogl/animation/skeleton_model.hpp"

namespace kreogl {
	struct KREOGL_EXPORT assimp_skeleton_model : skeleton_model {
		struct KREOGL_EXPORT mesh {
			struct KREOGL_EXPORT bone {
				const aiNode * node = nullptr;
				glm::mat4 offset{ 1.f };
			};
			std::vector<bone> bones;
		};

		const aiNode * root_node = nullptr;
		std::vector<mesh> assimp_meshes;
	};
}