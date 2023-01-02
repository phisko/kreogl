#include "gbuffer.hpp"

// stl
#include <cassert>

// gl
#include <GL/glew.h>
#include <GL/gl.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	gbuffer::gbuffer(const glm::ivec2 & size) noexcept
		: _size(size) {
		KREOGL_PROFILING_SCOPE;

		_textures.resize((int)texture::count);

		std::vector<GLenum> attachments;

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frame_buffer);

		for (size_t i = 0; i < _textures.size(); ++i) {
			glBindTexture(GL_TEXTURE_2D, _textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			const auto attachment_number = GL_COLOR_ATTACHMENT0 + i;
			glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachment_number, GL_TEXTURE_2D, _textures[i], 0);
			attachments.push_back((GLenum)attachment_number);
		}

		glBindTexture(GL_TEXTURE_2D, _depth_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_texture, 0);

		glDrawBuffers((GLsizei)attachments.size(), attachments.data());

		const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void gbuffer::resize(const glm::ivec2 & size) noexcept {
		KREOGL_PROFILING_SCOPE;

		if (_size == size)
			return;

		_size = size;

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frame_buffer);

		for (const auto & texture : _textures) {
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
		}

		glBindTexture(GL_TEXTURE_2D, _depth_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	void gbuffer::bind_for_writing() const noexcept {
		KREOGL_PROFILING_SCOPE;

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frame_buffer);
	}

	void gbuffer::bind_for_reading() const noexcept {
		KREOGL_PROFILING_SCOPE;

		glBindFramebuffer(GL_READ_FRAMEBUFFER, _frame_buffer);
		for (unsigned int i = 0; i < _textures.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, _textures[i]);
		}
	}

	void gbuffer::get_texture_content(gbuffer::texture texture, void * content) const noexcept {
		KREOGL_PROFILING_SCOPE;

		// Profiling results:
		// 		glReadPixels: 1ms
		//		glGetTexImage: 3.5ms
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _frame_buffer);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + int(texture));
		glReadPixels(0, 0, _size.x, _size.y, GL_RGBA, GL_FLOAT, content);
	}

	glm::vec4 gbuffer::read_pixel(gbuffer::texture texture, const glm::ivec2 & pixel) const noexcept {
		KREOGL_PROFILING_SCOPE;

		assert(pixel.x >= 0 && pixel.x < _size.x && pixel.y >= 0 && pixel.y < _size.y);

		static std::vector<glm::vec4> pixels;
		pixels.resize(_size.x * _size.y);
		get_texture_content(texture, pixels.data());

		const auto pixel_index = pixel.x + (_size.y - pixel.y) * _size.x;
		return pixels[pixel_index];
	}
}