#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT directional_light_glsl {
		static const char * frag;

		uniform<int> gposition{ "gposition" };
		uniform<int> gnormal{ "gnormal" };
		uniform<int> gdiffuse{ "gdiffuse" };
		uniform<int> gspecular{ "gspecular" };

		uniform<glm::vec3> view_pos{ "view_pos" };
		uniform<glm::vec2> screen_size{ "screen_size" };
		uniform<glm::vec4> color{ "color" };
		uniform<glm::vec3> direction{ "direction" };
		uniform<float> ambient_strength{ "ambient_strength" };
		uniform<float> diffuse_strength{ "diffuse_strength" };
		uniform<float> specular_strength{ "specular_strength" };

		struct KREOGL_EXPORT get_direction {
			static const char * frag;
			uniform<glm::vec3> direction{ "direction" };
		};
	};
}