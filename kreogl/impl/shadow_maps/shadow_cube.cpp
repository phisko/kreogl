#include "shadow_cube.hpp"

// stl
#include <cassert>

// kreogl
#include "kreogl/impl/raii/scoped_bind_framebuffer.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	shadow_cube::shadow_cube() noexcept {
		KREOGL_PROFILING_SCOPE;

		const scoped_bind_framebuffer bound(frame_buffer);

		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		for (size_t i = 0; i < 6; ++i)
			glTexImage2D((GLenum)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0, GL_DEPTH_COMPONENT, 1, 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}

	void shadow_cube::set_size(int size) noexcept {
		KREOGL_PROFILING_SCOPE;

		if (_size == size)
			return;
		_size = size;

		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		for (size_t i = 0; i < 6; ++i)
			glTexImage2D((GLenum)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
}