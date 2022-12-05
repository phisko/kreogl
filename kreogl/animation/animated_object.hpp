#pragma once

// stl
#include <optional>

// kreogl
#include "kreogl/object.hpp"
#include "skeleton.hpp"
#include "animation.hpp"

namespace kreogl {
	struct KREOGL_EXPORT animated_object : object {
		kreogl::skeleton skeleton;
		std::optional<kreogl::animation> animation;

		void tick_animation(float delta_time) noexcept;
	};
}