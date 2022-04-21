#pragma once

#include "UIntSwapper.hpp"

namespace kreogl {
    struct FrameBuffer : UIntSwapper {
        FrameBuffer() noexcept {
            glGenFramebuffers(1, &res);
        }

        ~FrameBuffer() noexcept {
            glDeleteFramebuffers(1, &res);
        }

        FrameBuffer(FrameBuffer &&) noexcept = default;
        FrameBuffer & operator=(FrameBuffer &&) noexcept = default;
    };
}