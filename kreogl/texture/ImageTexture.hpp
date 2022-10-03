#pragma once

#include <optional>
#include "kreogl/impl/RAII/Texture.hpp"

namespace kreogl {
    struct ImageTexture {
        ImageTexture(const char * file) noexcept;
        ImageTexture(const void * data, unsigned int width, unsigned int height) noexcept;

        ImageTexture() noexcept = default;
        ImageTexture(ImageTexture &&) noexcept = default;
        ImageTexture & operator=(ImageTexture &&) noexcept = default;

        static bool isSupportedFormat(const char * file) noexcept;

        int width = 0;
        int height = 0;
        int components = 0;
        std::optional<Texture> texture;
    };
}