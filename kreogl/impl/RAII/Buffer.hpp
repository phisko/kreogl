#pragma once

#include "UIntSwapper.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    struct Buffer : UIntSwapper {
        Buffer() noexcept {
            KREOGL_PROFILING_SCOPE;
            glGenBuffers(1, &res);
        }

        ~Buffer() noexcept {
            KREOGL_PROFILING_SCOPE;
            glDeleteBuffers(1, &res);
        }

        Buffer(Buffer &&) noexcept = default;
        Buffer & operator=(Buffer &&) noexcept = default;
    };
}