#pragma once

// stl
#include <optional>

// kreogl
#include "kreogl/Object.hpp"
#include "Skeleton.hpp"
#include "Animation.hpp"

namespace kreogl {
	struct KREOGL_EXPORT AnimatedObject : Object {
		Skeleton skeleton;
		std::optional<Animation> animation;

		void tickAnimation(float deltaTime) noexcept;
	};
}