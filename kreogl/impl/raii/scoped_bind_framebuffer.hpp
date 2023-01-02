#pragma once

// gl
#include <GL/glew.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT scoped_bind_framebuffer {
		scoped_bind_framebuffer(GLuint framebuffer) noexcept
			: _framebuffer(framebuffer) {
			KREOGL_PROFILING_SCOPE;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &_previous);
			glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
		}

		~scoped_bind_framebuffer() noexcept {
			KREOGL_PROFILING_SCOPE;
			glBindFramebuffer(GL_FRAMEBUFFER, _previous);
		}

		GLuint _framebuffer;
		GLint _previous;
	};
}