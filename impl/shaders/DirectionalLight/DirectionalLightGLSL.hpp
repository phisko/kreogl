#pragma once

#include "impl/shaders/Uniform.hpp"

namespace kreogl {
    struct DirectionalLightGLSL {
        static const char * frag;
        Uniform<glm::vec3> viewPos{ "viewPos" };
        Uniform<glm::vec2> screenSize{ "screenSize" };
        Uniform<glm::vec4> color{ "color" };
        Uniform<glm::vec3> direction{ "direction" };
        Uniform<float> ambientStrength{ "ambientStrength" };
        Uniform<float> diffuseStrength{ "diffuseStrength" };
        Uniform<float> specularStrength{ "specularStrength" };

        static const char * getDirection;
    };
}