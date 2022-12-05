#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"
#include "kreogl/impl/shadow_maps/cascaded_shadow_map.hpp"

namespace kreogl {
	struct KREOGL_EXPORT sample_cascaded_shadow_map_glsl {
		static const char * frag;
		uniform<size_t> shadow_map[KREOGL_MAX_CSM_COUNT];
		uniform<glm::mat4> light_space_matrix[KREOGL_MAX_CSM_COUNT];
		uniform<float> cascade_end[KREOGL_MAX_CSM_COUNT];
		uniform<int> cascade_count{ "cascade_count" };
		uniform<float> min_bias{ "min_bias" };
		uniform<float> max_bias{ "max_bias" };
		uniform<int> pcf_samples{ "pcf_samples" };
		uniform<glm::mat4> view{ "view" };

		sample_cascaded_shadow_map_glsl() noexcept {
			for (size_t i = 0; i < KREOGL_MAX_CSM_COUNT; ++i) {
				const auto index = std::to_string(i);
				shadow_map[i].name = "shadow_map[" + index + "]";
				light_space_matrix[i].name = "light_space_matrix[" + index + "]";
				cascade_end[i].name = "cascade_end[" + index + "]";
			}
		}
	};
}