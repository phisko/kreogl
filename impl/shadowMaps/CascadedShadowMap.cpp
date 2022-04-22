#include "CascadedShadowMap.hpp"

#include <cassert>
#include "impl/RAII/ScopedBindFramebuffer.hpp"

namespace kreogl {
    CascadedShadowMap::CascadedShadowMap() noexcept {
        const ScopedBindFramebuffer bound(frameBuffer);

        for (const auto & texture : textures) {
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1, 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            const float borderColor[] = { 1.f, 1.f, 1.f, 1.f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[0], 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }

    void CascadedShadowMap::setSize(int size) noexcept {
        if (_size == size)
            return;
        _size = size;

        for (const auto & texture : textures) {
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (GLsizei)size, (GLsizei)size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        }
    }
}