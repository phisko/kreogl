#pragma once

#include "UIntSwapper.hpp"

namespace kreogl {
    struct Texture : UIntSwapper {
        Texture() noexcept {
            glGenTextures(1, &res);
        }

        ~Texture() noexcept {
            glDeleteTextures(1, &res);
        }

        Texture(Texture &&) noexcept = default;
        Texture & operator=(Texture &&) noexcept = default;
    };
}