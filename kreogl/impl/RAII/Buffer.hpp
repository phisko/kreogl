#pragma once

#include "UIntSwapper.hpp"

namespace kreogl {
    struct Buffer : UIntSwapper {
        Buffer() noexcept {
            glGenBuffers(1, &res);
        }

        ~Buffer() noexcept {
            glDeleteBuffers(1, &res);
        }

        Buffer(Buffer &&) noexcept = default;
        Buffer & operator=(Buffer &&) noexcept = default;
    };
}