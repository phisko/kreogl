#pragma once

#include <variant>
#include <glm/glm.hpp>

namespace kreogl {
	struct KREOGL_EXPORT DebugElement {
		enum class Type {
			Line,
			Sphere,
			Box
		};

		Type type;
		glm::mat4 transform{ 1.f };
		glm::vec4 color{ 1.f };
		glm::vec4 userData{ 0.f };
		glm::vec3 lineStart{ 0.f, 0.f, 0.f };
		glm::vec3 lineEnd{ 0.f, 0.f, 1.f };
	};
}