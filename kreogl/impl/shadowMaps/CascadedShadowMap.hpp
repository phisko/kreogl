#pragma once

#include "kreogl/impl/RAII/FrameBuffer.hpp"
#include "kreogl/impl/RAII/Texture.hpp"

#ifndef KREOGL_MAX_CSM_COUNT
# define KREOGL_MAX_CSM_COUNT 5
#endif

namespace kreogl {
    struct CascadedShadowMap {
        CascadedShadowMap() noexcept;

        FrameBuffer frameBuffer;
        Texture textures[KREOGL_MAX_CSM_COUNT];

        void setSize(int size) noexcept;
        int getSize() const noexcept { return _size; }

    private:
        int _size = -1;
    };
}