#pragma once

// stl
#include <string>

// glm
#include <glm/glm.hpp>

namespace kreogl {
	struct KREOGL_EXPORT text {
		std::string value;
		std::string font;
		size_t font_size = 1024;

		enum class alignment_type {
			left,
			center,
			right
		};
		alignment_type alignment = alignment_type::center;

		glm::mat4 transform{ 1.f };
		glm::vec4 color{ 1.f };
		glm::vec4 user_data{ 0.f };
	};

	struct KREOGL_EXPORT text_2d : text {};
	struct KREOGL_EXPORT text_3d : text {};
}