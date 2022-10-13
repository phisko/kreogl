#pragma once

#include "kreogl/impl/shaders/Uniform.hpp"

namespace kreogl {
    struct ColorGLSL {
        static const char * frag;
        Uniform<glm::vec4> color = { "color" };
    };
}