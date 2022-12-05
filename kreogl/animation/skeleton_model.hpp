#pragma once

// stl
#include <string>
#include <vector>

namespace kreogl {
	struct KREOGL_EXPORT skeleton_model {
		virtual ~skeleton_model() noexcept = default;

		struct mesh {
			std::vector<std::string> bone_names;
		};

		std::vector<mesh> meshes;
	};
}