#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT text_glsl {
		static const char * vert;
		uniform<glm::mat4> model{ "model" };
		uniform<glm::mat4> view{ "view" };
		uniform<glm::mat4> proj{ "proj" };
		uniform<glm::vec3> view_pos{ "view_pos" };

		static const char * frag;
		uniform<size_t> tex{ "tex" };
		uniform<glm::vec4> color{ "color" };
		uniform<glm::vec4> user_data{ "user_data" };
	};
}