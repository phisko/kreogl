#pragma once

// kreogl
#include "kreogl/impl/shaders/uniform.hpp"
#include "kreogl/impl/shadow_maps/shadow_cube.hpp"

namespace kreogl {
	struct KREOGL_EXPORT shadow_cube_glsl {
		static const char * geom;
		uniform<glm::mat4> light_space_matrices[6];

		static const char * frag;
		uniform<glm::vec3> light_pos{ "light_pos" };
		uniform<float> far_plane{ "far_plane" };

		shadow_cube_glsl() noexcept {
			for (size_t i = 0; i < 6; ++i) {
				const auto index = std::to_string(i);
				light_space_matrices[i].name = "light_space_matrices[" + index + "]";
			}
		}
	};
}