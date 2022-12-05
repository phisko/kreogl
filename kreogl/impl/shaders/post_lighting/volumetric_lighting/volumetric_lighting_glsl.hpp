#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT volumetric_lighting_glsl {
		static const char * frag;

		uniform<int> gposition{ "gposition" };
		uniform<int> gdiffuse{ "gdiffuse" };

		uniform<float> scattering{ "scattering" };
		uniform<int> nb_steps{ "nb_steps" };
		uniform<float> default_step_length{ "default_step_length" };
		uniform<float> intensity{ "intensity" };

		uniform<glm::mat4> inverse_view{ "inverse_view" };
		uniform<glm::mat4> inverse_proj{ "inverse_proj" };
		uniform<glm::vec3> view_pos{ "view_pos" };
		uniform<glm::vec2> screen_size{ "screen_size" };

		uniform<glm::vec4> color{ "color" };
	};
}