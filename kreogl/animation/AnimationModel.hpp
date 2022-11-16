#pragma once

#include <string>

namespace kreogl {
	struct KREOGL_EXPORT AnimationModel {
		virtual ~AnimationModel() noexcept = default;

		virtual void tick(float deltaTime, struct AnimatedObject & object) const noexcept = 0;

		std::string name;
		float totalTime = 0.f;
		float ticksPerSecond = 0.f;
	};
}