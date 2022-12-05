#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT spot_light_glsl {
		static const char * frag;

		uniform<int> gposition{ "gposition" };
		uniform<int> gnormal{ "gnormal" };
		uniform<int> gdiffuse{ "gdiffuse" };
		uniform<int> gspecular{ "gspecular" };

		uniform<glm::vec3> view_pos{ "view_pos" };
		uniform<glm::vec2> screen_size{ "screen_size" };
		uniform<glm::vec4> color{ "color" };
		uniform<glm::vec3> position{ "position" };
		uniform<glm::vec3> direction{ "direction" };
		uniform<float> cut_off{ "cut_off" };
		uniform<float> outer_cut_off{ "outer_cut_off" };
		uniform<float> diffuse_strength{ "diffuse_strength" };
		uniform<float> specular_strength{ "specular_strength" };
		uniform<float> attenuation_constant{ "attenuation_constant" };
		uniform<float> attenuation_linear{ "attenuation_linear" };
		uniform<float> attenuation_quadratic{ "attenuation_quadratic" };
	};
}