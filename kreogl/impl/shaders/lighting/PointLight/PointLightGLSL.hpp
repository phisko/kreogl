#pragma once

#include "kreogl/impl/shaders/Uniform.hpp"

namespace kreogl {
    struct PointLightGLSL {
        static const char * frag;

        Uniform<int> gposition{ "gposition" };
        Uniform<int> gnormal{ "gnormal" };
        Uniform<int> gdiffuse{ "gdiffuse" };
        Uniform<int> gspecular{ "gspecular" };

        Uniform<glm::vec3> viewPos{ "viewPos" };
        Uniform<glm::vec2> screenSize{ "screenSize" };
        Uniform<glm::vec4> color{ "color" };
        Uniform<glm::vec3> position{ "position" };
        Uniform<float> diffuseStrength{ "diffuseStrength" };
        Uniform<float> specularStrength{ "specularStrength" };
        Uniform<float> attenuationConstant{ "attenuationConstant" };
        Uniform<float> attenuationLinear{ "attenuationLinear" };
        Uniform<float> attenuationQuadratic{ "attenuationQuadratic" };

        struct GetDirection {
            static const char * frag;
            Uniform<glm::vec3> position{ "position" };
        };
    };
}
