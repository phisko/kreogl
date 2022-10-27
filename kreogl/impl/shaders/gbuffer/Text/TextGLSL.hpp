#pragma once

#include "kreogl/impl/shaders/Uniform.hpp"

namespace kreogl {
	struct TextGLSL {
		static const char * vert;
		Uniform<glm::mat4> model{ "model" };
		Uniform<glm::mat4> view{ "view" };
		Uniform<glm::mat4> proj{ "proj" };
		Uniform<glm::vec3> viewPos{ "viewPos" };

		static const char * frag;
		Uniform<size_t> tex{ "tex" };
		Uniform<glm::vec4> color{ "color" };
		Uniform<glm::vec4> userData{ "userData" };
	};
}