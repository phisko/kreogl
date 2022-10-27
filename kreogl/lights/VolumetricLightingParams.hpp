#pragma once

namespace kreogl {
	struct VolumetricLightingParams {
		float scattering = .1f;
		int nbSteps = 10;
		float defaultStepLength = 25.f;
		float intensity = 2.5f;
	};
}