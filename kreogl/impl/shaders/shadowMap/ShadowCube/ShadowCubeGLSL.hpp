#pragma once

#include "kreogl/impl/shaders/Uniform.hpp"
#include "kreogl/impl/shadowMaps/ShadowCube.hpp"

namespace kreogl {
    struct ShadowCubeGLSL {
        static const char * geom;

        Uniform<glm::mat4> lightSpaceMatrices[6];

        static const char * frag;

        Uniform<glm::vec3> lightPos{ "lightPos" };
        Uniform<float> farPlane{ "farPlane" };

        ShadowCubeGLSL() noexcept {
            for (size_t i = 0; i < 6; ++i) {
                const auto index = std::to_string(i);
                lightSpaceMatrices[i].name = "lightSpaceMatrices[" + index + "]";
            }
        }
    };
}