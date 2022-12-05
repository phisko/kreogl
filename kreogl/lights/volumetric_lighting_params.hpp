#pragma once

namespace kreogl {
	struct KREOGL_EXPORT volumetric_lighting_params {
		float scattering = .1f;
		int nb_steps = 10;
		float default_step_length = 25.f;
		float intensity = 2.5f;
	};
}