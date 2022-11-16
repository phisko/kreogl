#pragma once

#include "Light.hpp"
#include "kreogl/impl/shadowMaps/CascadedShadowMap.hpp"
#include "kreogl/impl/DrawParams.hpp"

namespace kreogl {
	struct KREOGL_EXPORT DirectionalLight : Light {
		DirectionalLight() noexcept {
			lightSphereSize = 100.f;
		}

		glm::vec3 direction{ 0, -1, 0 };
		float ambientStrength = .1f;
		float lightSphereDistance = 500.f;

		std::vector<float> cascadeEnds = { 5, 10, 20, 40, 100 };
		float shadowCasterMaxDistance = 100.f;
		mutable CascadedShadowMap cascadedShadowMap;

		glm::mat4 getLightSpaceMatrixForCascade(const DrawParams & params, size_t index) const noexcept;
	};
}