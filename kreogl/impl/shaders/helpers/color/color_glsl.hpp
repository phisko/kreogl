#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT color_glsl {
		static const char * frag;
		uniform<glm::vec4> color = { "color" };
	};
}