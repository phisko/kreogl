#pragma once

// stl
#include <vector>
#include <memory>

// kreogl
#include "animation_model.hpp"
#include "kreogl/impl/default_constructors.hpp"

namespace kreogl {
	struct KREOGL_EXPORT animation_file {
		KREOGL_MOVE_ONLY(animation_file);

		virtual ~animation_file() noexcept = default;
		std::vector<std::unique_ptr<animation_model>> animations;
	};
}