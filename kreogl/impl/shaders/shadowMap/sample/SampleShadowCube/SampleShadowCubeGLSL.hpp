#pragma once

#include "kreogl/impl/shaders/Uniform.hpp"

namespace kreogl {
	struct SampleShadowCubeGLSL {
		static const char * frag;

		Uniform<size_t> shadowMap{ "shadowMap" };
		Uniform<glm::vec3> position{ "position" };
		Uniform<glm::vec3> viewPos{ "viewPos" };
		Uniform<float> farPlane{ "farPlane" };
		Uniform<float> minBias{ "minBias" };
		Uniform<float> maxBias{ "maxBias" };
	};
}