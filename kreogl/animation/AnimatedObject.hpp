#pragma once

#include <optional>

#include "kreogl/Object.hpp"
#include "Skeleton.hpp"
#include "Animation.hpp"

namespace kreogl {
	struct AnimatedObject : Object {
		Skeleton skeleton;
		std::optional<Animation> animation;

		void tickAnimation(float deltaTime) noexcept;
	};
}