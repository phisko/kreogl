#pragma once

#include <string>
#include <vector>

namespace kreogl {
	struct KREOGL_EXPORT SkeletonModel {
		virtual ~SkeletonModel() noexcept = default;

		struct Mesh {
			std::vector<std::string> boneNames;
		};

		std::vector<Mesh> meshes;
	};
}