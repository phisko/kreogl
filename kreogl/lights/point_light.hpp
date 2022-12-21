#pragma once

// kreogl
#include "light.hpp"
#include "kreogl/impl/shadow_maps/shadow_cube.hpp"

namespace kreogl {
	struct KREOGL_EXPORT point_light : light {
		glm::vec3 position;
		float attenuation_constant = 1.f;
		float attenuation_linear = .09f;
		float attenuation_quadratic = .032f;

		float get_radius() const noexcept;

		mutable kreogl::shadow_cube shadow_cube;
	};
}