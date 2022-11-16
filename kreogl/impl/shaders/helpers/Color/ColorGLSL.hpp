#pragma once

// kreogl
#include "kreogl/impl/shaders/Uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT ColorGLSL {
		static const char * frag;
		Uniform<glm::vec4> color = { "color" };
	};
}