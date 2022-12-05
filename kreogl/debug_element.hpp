#pragma once

// glm
#include <glm/glm.hpp>

namespace kreogl {
	struct KREOGL_EXPORT debug_element {
		enum class type {
			line,
			sphere,
			box
		};

		type type;
		glm::mat4 transform{ 1.f };
		glm::vec4 color{ 1.f };
		glm::vec4 user_data{ 0.f };
		glm::vec3 line_start{ 0.f, 0.f, 0.f };
		glm::vec3 line_end{ 0.f, 0.f, 1.f };
	};
}