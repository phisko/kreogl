#pragma once

#include "kreogl/impl/shaders/Uniform.hpp"

namespace kreogl {
	struct KREOGL_EXPORT VolumetricLightingGLSL {
		static const char * frag;

		Uniform<int> gposition{ "gposition" };
		Uniform<int> gdiffuse{ "gdiffuse" };

		Uniform<float> scattering{ "scattering" };
		Uniform<int> nbSteps{ "nbSteps" };
		Uniform<float> defaultStepLength{ "defaultStepLength" };
		Uniform<float> intensity{ "intensity" };

		Uniform<glm::mat4> inverseView{ "inverseView" };
		Uniform<glm::mat4> inverseProj{ "inverseProj" };
		Uniform<glm::vec3> viewPos{ "viewPos" };
		Uniform<glm::vec2> screenSize{ "screenSize" };

		Uniform<glm::vec4> color{ "color" };
	};
}