#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT position_proj_view_model_glsl {
		static const char * vert;
		uniform<glm::mat4> proj = { "proj" };
		uniform<glm::mat4> view = { "view" };
		uniform<glm::mat4> model = { "model" };
	};
}