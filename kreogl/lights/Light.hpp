#pragma once

#include <vector>
#include <optional>
#include <glm/glm.hpp>
#include "VolumetricLightingParams.hpp"

#ifndef KREOGL_SHADOW_MAP_NEAR_PLANE
#	define KREOGL_SHADOW_MAP_NEAR_PLANE .1f
#endif

#ifndef KREOGL_SHADOW_MAP_FAR_PLANE
#	define KREOGL_SHADOW_MAP_FAR_PLANE 1000.f
#endif

namespace kreogl {
	struct KREOGL_EXPORT Light {
		glm::vec4 color{ 1.f };
		float diffuseStrength = 1.f;
		float specularStrength = 1.f;

		bool castShadows = true;
		int shadowPCFSamples = 1;
		int shadowMapSize = 1024;
		float shadowMapMaxBias = .1f;
		float shadowMapMinBias = .01f;
		std::optional<VolumetricLightingParams> volumetricLighting;

		float lightSphereSize = .25f;
	};
}