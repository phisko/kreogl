#include "ShadowMap.hpp"

// stl
#include <cassert>

// kreogl
#include "kreogl/impl/RAII/ScopedBindFramebuffer.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	ShadowMap::ShadowMap() noexcept {
		KREOGL_PROFILING_SCOPE;

		const ScopedBindFramebuffer bound(frameBuffer);

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1, 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		const float borderColor[] = { 1.f, 1.f, 1.f, 1.f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}

	void ShadowMap::setSize(int size) noexcept {
		KREOGL_PROFILING_SCOPE;

		if (_size == size)
			return;
		_size = size;

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, (GLsizei)size, (GLsizei)size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}
}