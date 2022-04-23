#pragma once

#include <GL/glew.h>

namespace kreogl {
    struct ScopedGLFeature {
        ScopedGLFeature(GLenum feature) noexcept
            : feature(feature)
        {
            glEnable(feature);
        }

        ~ScopedGLFeature()
        {
            glDisable(feature);
        }

        GLenum feature;
    };
}