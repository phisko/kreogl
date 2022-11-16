#pragma once

namespace kreogl {
	struct KREOGL_EXPORT VolumetricLightingParams {
		float scattering = .1f;
		int nbSteps = 10;
		float defaultStepLength = 25.f;
		float intensity = 2.5f;
	};
}