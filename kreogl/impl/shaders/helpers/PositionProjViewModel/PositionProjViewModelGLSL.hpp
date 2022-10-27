#pragma once

#include "kreogl/impl/shaders/Uniform.hpp"

namespace kreogl {
	struct PositionProjViewModelGLSL {
		static const char * vert;
		Uniform<glm::mat4> proj = { "proj" };
		Uniform<glm::mat4> view = { "view" };
		Uniform<glm::mat4> model = { "model" };
	};
}