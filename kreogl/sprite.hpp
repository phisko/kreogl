#pragma once

// glm
#include <glm/glm.hpp>

// kreogl
#include "impl/raii/texture.hpp"

namespace kreogl {
	struct KREOGL_EXPORT sprite {
		const texture * tex = nullptr;
		glm::mat4 transform{ 1.f };
		glm::vec4 color{ 1.f };
		glm::vec4 user_data{ 0.f };
	};

	struct KREOGL_EXPORT sprite_2d : sprite {};
	struct KREOGL_EXPORT sprite_3d : sprite {};
}