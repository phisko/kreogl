#pragma once

namespace kreogl {
	enum class shader_step {
		// deferred
		gbuffer,
		lighting,
		post_lighting,
		post_process,
		// shadow maps
		shadow_map,
		shadow_cube
	};
}