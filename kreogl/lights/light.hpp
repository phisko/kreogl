#pragma once

// stl
#include <vector>
#include <optional>

// glm
#include <glm/glm.hpp>

// kreogl
#include "volumetric_lighting_params.hpp"

#ifndef KREOGL_SHADOW_MAP_NEAR_PLANE
#	define KREOGL_SHADOW_MAP_NEAR_PLANE .1f
#endif

#ifndef KREOGL_SHADOW_MAP_FAR_PLANE
#	define KREOGL_SHADOW_MAP_FAR_PLANE 1000.f
#endif

namespace kreogl {
	struct KREOGL_EXPORT light {
		glm::vec4 color{ 1.f };
		float diffuse_strength = 1.f;
		float specular_strength = 1.f;

		bool cast_shadows = true;
		int shadow_pcf_samples = 1;
		int shadow_map_size = 1024;
		float shadow_map_max_bias = .1f;
		float shadow_map_min_bias = .01f;
		std::optional<volumetric_lighting_params> volumetric_lighting;

		float light_sphere_size = .25f;
	};
}