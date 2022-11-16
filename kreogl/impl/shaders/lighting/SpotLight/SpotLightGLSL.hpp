#pragma once

// kreogl
#include "kreogl/impl/shaders/Uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT SpotLightGLSL {
		static const char * frag;

		Uniform<int> gposition{ "gposition" };
		Uniform<int> gnormal{ "gnormal" };
		Uniform<int> gdiffuse{ "gdiffuse" };
		Uniform<int> gspecular{ "gspecular" };

		Uniform<glm::vec3> viewPos{ "viewPos" };
		Uniform<glm::vec2> screenSize{ "screenSize" };
		Uniform<glm::vec4> color{ "color" };
		Uniform<glm::vec3> position{ "position" };
		Uniform<glm::vec3> direction{ "direction" };
		Uniform<float> cutOff{ "cutOff" };
		Uniform<float> outerCutOff{ "outerCutOff" };
		Uniform<float> diffuseStrength{ "diffuseStrength" };
		Uniform<float> specularStrength{ "specularStrength" };
		Uniform<float> attenuationConstant{ "attenuationConstant" };
		Uniform<float> attenuationLinear{ "attenuationLinear" };
		Uniform<float> attenuationQuadratic{ "attenuationQuadratic" };
	};
}