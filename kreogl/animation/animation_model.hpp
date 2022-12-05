#pragma once

// stl
#include <string>

namespace kreogl {
	struct KREOGL_EXPORT animation_model {
		virtual ~animation_model() noexcept = default;

		virtual void tick(float delta_time, struct animated_object & object) const noexcept = 0;

		std::string name;
		float total_time = 0.f;
		float ticks_per_second = 0.f;
	};
}