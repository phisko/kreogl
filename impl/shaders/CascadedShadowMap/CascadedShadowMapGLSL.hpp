#pragma once

#include "impl/shaders/Uniform.hpp"
#include "impl/shadowMaps/CascadedShadowMap.hpp"

namespace kreogl {
    struct CascadedShadowMapGLSL {
        static const char * frag;
        Uniform<size_t> shadowMap[KREOGL_MAX_CSM_COUNT];
        Uniform<glm::mat4> lightSpaceMatrix[KREOGL_MAX_CSM_COUNT];
        Uniform<float> cascadeEnd[KREOGL_MAX_CSM_COUNT];
        Uniform<int> cascadeCount{ "cascadeCount" };
        Uniform<float> bias{ "bias" };
        Uniform<int> pcfSamples{ "pcfSamples" };
        Uniform<glm::mat4> proj{ "proj" };
        Uniform<glm::mat4> view{ "view" };

        CascadedShadowMapGLSL() noexcept {
            for (size_t i = 0; i < KREOGL_MAX_CSM_COUNT; ++i) {
                const auto index = std::to_string(i);
                shadowMap[i].name = "shadowMap[" + index + "]";
                lightSpaceMatrix[i].name = "lightSpaceMatrix[" + index + "]";
                cascadeEnd[i].name = "cascadeEnd[" + index + "]";
            }
        }
    };
}