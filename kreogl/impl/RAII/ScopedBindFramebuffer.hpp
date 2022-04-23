#pragma once

#include <GL/glew.h>

namespace kreogl {
    struct ScopedBindFramebuffer {
        ScopedBindFramebuffer(GLuint framebuffer) noexcept
            : _framebuffer(framebuffer)
        {
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_previous);
            glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
        }

        ~ScopedBindFramebuffer() noexcept {
            glBindFramebuffer(GL_FRAMEBUFFER, _previous);
        }

        GLuint _framebuffer;
        GLint _previous;
    };
}