#pragma once

// kreogl
#include "point_light.hpp"
#include "kreogl/impl/shadow_maps/shadow_map.hpp"
#include "kreogl/impl/draw_params.hpp"

namespace kreogl {
	struct KREOGL_EXPORT spot_light : point_light {
		glm::vec3 direction;
		float cut_off = .91f;
		float outer_cut_off = .82f;

		mutable kreogl::shadow_map shadow_map;

		glm::mat4 get_light_space_matrix() const noexcept;
	};
}