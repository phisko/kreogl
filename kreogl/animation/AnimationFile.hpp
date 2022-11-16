#pragma once

// stl
#include <vector>
#include <memory>

// kreogl
#include "AnimationModel.hpp"

namespace kreogl {
	struct KREOGL_EXPORT AnimationFile {
		virtual ~AnimationFile() noexcept = default;
		std::vector<std::unique_ptr<AnimationModel>> animations;
	};
}