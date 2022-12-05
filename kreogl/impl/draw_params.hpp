#pragma once

namespace kreogl {
	struct KREOGL_EXPORT draw_params {
		const class world & world;
		const class camera & camera;
		const class shader_pipeline & shader_pipeline;
	};
}