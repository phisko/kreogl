#pragma once

namespace kreogl {
	struct KREOGL_EXPORT DrawParams {
		const class World & world;
		const class Camera & camera;
		const class ShaderPipeline & shaderPipeline;
	};
}