#include "cascaded_shadow_map.hpp"

// stl
#include <cassert>

// kreogl
#include "kreogl/impl/raii/scoped_bind_framebuffer.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	cascaded_shadow_map::cascaded_shadow_map() noexcept {
		KREOGL_PROFILING_SCOPE;

		const scoped_bind_framebuffer bound(frame_buffer);

		for (const auto & texture : textures) {
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1, 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			const float border_color[] = { 1.f, 1.f, 1.f, 1.f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[0], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}

	void cascaded_shadow_map::set_size(int size) noexcept {
		KREOGL_PROFILING_SCOPE;

		if (_size == size)
			return;
		_size = size;

		for (const auto & texture : textures) {
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (GLsizei)size, (GLsizei)size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		}
	}
}