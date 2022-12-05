#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT sample_shadow_cube_glsl {
		static const char * frag;

		uniform<size_t> shadow_map{ "shadow_map" };
		uniform<glm::vec3> position{ "position" };
		uniform<glm::vec3> view_pos{ "view_pos" };
		uniform<float> far_plane{ "far_plane" };
		uniform<float> min_bias{ "min_bias" };
		uniform<float> max_bias{ "max_bias" };
	};
}