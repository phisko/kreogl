#pragma once

#include "UIntSwapper.hpp"

namespace kreogl {
    struct VertexArray : UIntSwapper {
        VertexArray() noexcept {
            glGenVertexArrays(1, &res);
        }

        ~VertexArray() noexcept {
            glDeleteVertexArrays(1, &res);
        }

        VertexArray(VertexArray &&) noexcept = default;
        VertexArray & operator=(VertexArray &&) noexcept = default;
    };
}