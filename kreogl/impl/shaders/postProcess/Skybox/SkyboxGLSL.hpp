#pragma once

// kreogl
#include "kreogl/impl/shaders/Uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT SkyboxGLSL {
		static const char * vert;
		Uniform<glm::mat4> proj{ "proj" };
		Uniform<glm::mat4> view{ "view" };

		static const char * frag;
		Uniform<size_t> tex{ "tex" };
		Uniform<glm::vec4> color{ "color" };
	};
}