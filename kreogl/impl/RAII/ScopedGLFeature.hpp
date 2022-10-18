#pragma once

#include <GL/glew.h>
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    struct ScopedGLFeature {
        ScopedGLFeature(GLenum feature) noexcept
            : feature(feature)
        {
            KREOGL_PROFILING_SCOPE;
            glEnable(feature);
        }

        ~ScopedGLFeature()
        {
            KREOGL_PROFILING_SCOPE;
            glDisable(feature);
        }

        GLenum feature;
    };
}