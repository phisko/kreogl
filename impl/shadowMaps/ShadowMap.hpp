#pragma once

#include "impl/RAII/FrameBuffer.hpp"
#include "impl/RAII/Texture.hpp"

namespace kreogl {
    struct ShadowMap {
        ShadowMap() noexcept;

        FrameBuffer frameBuffer;
        Texture texture;

        void setSize(int size) noexcept;
        int getSize() const noexcept { return _size; }

    private:
        int _size = -1;
    };
}