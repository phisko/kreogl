#pragma once

#include "kreogl/impl/shaders/Uniform.hpp"

namespace kreogl {
    struct DirectionalLightGLSL {
        static const char * frag;

        Uniform<int> gposition{ "gposition" };
        Uniform<int> gnormal{ "gnormal" };
        Uniform<int> gdiffuse{ "gdiffuse" };
        Uniform<int> gspecular{ "gspecular" };

        Uniform<glm::vec3> viewPos{ "viewPos" };
        Uniform<glm::vec2> screenSize{ "screenSize" };
        Uniform<glm::vec4> color{ "color" };
        Uniform<glm::vec3> direction{ "direction" };
        Uniform<float> ambientStrength{ "ambientStrength" };
        Uniform<float> diffuseStrength{ "diffuseStrength" };
        Uniform<float> specularStrength{ "specularStrength" };

        struct GetDirection {
            static const char * frag;
        };
    };
}