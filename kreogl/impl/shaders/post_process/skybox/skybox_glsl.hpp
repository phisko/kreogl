#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT skybox_glsl {
		static const char * vert;
		uniform<glm::mat4> proj{ "proj" };
		uniform<glm::mat4> view{ "view" };

		static const char * frag;
		uniform<size_t> tex{ "tex" };
		uniform<glm::vec4> color{ "color" };
	};
}