#pragma once

#include "kreogl/impl/shaders/Uniform.hpp"

namespace kreogl {
    struct SampleShadowMapGLSL {
        static const char * frag;

        Uniform<size_t> shadowMap{ "shadowMap" };
        Uniform<glm::mat4> lightSpaceMatrix{ "lightSpaceMatrix" };
        Uniform<float> minBias{ "minBias" };
        Uniform<float> maxBias{ "maxBias" };
        Uniform<int> pcfSamples{ "pcfSamples" };
    };
}