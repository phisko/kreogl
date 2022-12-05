#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT sample_shadow_map_glsl {
		static const char * frag;

		uniform<size_t> shadow_map{ "shadow_map" };
		uniform<glm::mat4> light_space_matrix{ "light_space_matrix" };
		uniform<float> min_bias{ "min_bias" };
		uniform<float> max_bias{ "max_bias" };
		uniform<int> pcf_samples{ "pcf_samples" };
	};
}