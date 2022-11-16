#pragma once

// kreogl
#include "Light.hpp"
#include "kreogl/impl/shadowMaps/ShadowCube.hpp"

namespace kreogl {
	struct KREOGL_EXPORT PointLight : Light {
		glm::vec3 position;
		float range = 1000.f;
		float attenuationConstant = 1.f;
		float attenuationLinear = .09f;
		float attenuationQuadratic = .032f;

		float getRadius() const noexcept;

		mutable ShadowCube shadowCube;
	};
}