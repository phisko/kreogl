#pragma once

// stl
#include <vector>
#include <memory>

// kreogl
#include "AnimationModel.hpp"
#include "kreogl/impl/DefaultConstructors.hpp"

namespace kreogl {
	struct KREOGL_EXPORT AnimationFile {
		KREOGL_MOVE_ONLY(AnimationFile);

		virtual ~AnimationFile() noexcept = default;
		std::vector<std::unique_ptr<AnimationModel>> animations;
	};
}