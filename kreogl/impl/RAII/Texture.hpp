#pragma once

#include <optional>
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

    bool isSupporteTextureFormat(const char * file) noexcept;
    std::optional<Texture> loadTexture(const char * file) noexcept;
    std::optional<Texture> loadTexture(const void * data, size_t width, size_t height) noexcept;
}