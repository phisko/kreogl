#pragma once

// glm
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

// kreogl
#include "animation_model.hpp"

namespace kreogl {
	struct KREOGL_EXPORT animation {
		const animation_model * model = nullptr;
		float current_time = 0.f;
		float speed = 1.f;
		bool loop = false;

		enum class mover_behavior {
			update_transform,
			update_bones,
			none
		};

		mover_behavior position_mover_behavior = mover_behavior::update_bones;
		mover_behavior rotation_mover_behavior = mover_behavior::update_bones;
		mover_behavior scale_mover_behavior = mover_behavior::none;

		struct movement_up_to_frame {
			glm::vec3 position{ 0.f };
			glm::quat rotation{ 0.f, 0.f, 0.f, 0.f };
			glm::vec3 scale{ 1.f };
		};
		movement_up_to_frame movement_up_to_last_frame;
	};
}