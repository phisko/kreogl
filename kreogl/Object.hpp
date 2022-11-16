#pragma once

#include <glm/glm.hpp>
#include "model/Model.hpp"

namespace kreogl {
	struct KREOGL_EXPORT Object {
		const Model * model;
		glm::mat4 transform{ 1.f };
		glm::vec4 color{ 1.f };
		glm::vec4 userData{ 0.f };
		bool castShadows = true;
	};
}