#pragma once

// kreogl
#include "light.hpp"
#include "kreogl/impl/shadow_maps/cascaded_shadow_map.hpp"
#include "kreogl/impl/draw_params.hpp"

namespace kreogl {
	struct KREOGL_EXPORT directional_light : light {
		directional_light() noexcept {
			light_sphere_size = 100.f;
		}

		glm::vec3 direction{ 0, -1, 0 };
		float ambient_strength = .1f;
		float light_sphere_distance = 500.f;

		std::vector<float> cascade_ends = { 5, 10, 20, 40, 100 };
		float shadow_caster_max_distance = 100.f;
		mutable kreogl::cascaded_shadow_map cascaded_shadow_map;

		glm::mat4 get_light_space_matrix_for_cascade(const draw_params & params, size_t index) const noexcept;
	};
}