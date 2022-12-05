#pragma once

// glm
#include <glm/glm.hpp>

// kreogl
#include "model/model.hpp"

namespace kreogl {
	struct KREOGL_EXPORT object {
		const kreogl::model * model;
		glm::mat4 transform{ 1.f };
		glm::vec4 color{ 1.f };
		glm::vec4 user_data{ 0.f };
		bool cast_shadows = true;
	};
}