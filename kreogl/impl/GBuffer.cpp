#include "GBuffer.hpp"

#include <cassert>

#include <GL/glew.h>
#include <GL/gl.h>

#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    GBuffer::GBuffer(const glm::ivec2 &size) noexcept
        : _size(size)
    {
        KREOGL_PROFILING_SCOPE;

        _textures.resize((int)Texture::Count);

        std::vector<GLenum> attachments;

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBuffer);

        for (size_t i = 0; i < _textures.size(); ++i) {
            glBindTexture(GL_TEXTURE_2D, _textures[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            const auto attachmentNumber = GL_COLOR_ATTACHMENT0 + i;
            glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)attachmentNumber, GL_TEXTURE_2D, _textures[i], 0);
            attachments.push_back((GLenum)attachmentNumber);
        }

        glBindTexture(GL_TEXTURE_2D, _depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);

        glDrawBuffers((GLsizei)attachments.size(), attachments.data());

        const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    void GBuffer::resize(const glm::ivec2 &size) noexcept {
        KREOGL_PROFILING_SCOPE;

        if (_size == size)
            return;

        _size = size;

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBuffer);

        for (const auto & texture : _textures) {
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, nullptr);
        }

        glBindTexture(GL_TEXTURE_2D, _depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    void GBuffer::bindForWriting() const noexcept {
        KREOGL_PROFILING_SCOPE;

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBuffer);
    }

    void GBuffer::bindForReading() const noexcept {
        KREOGL_PROFILING_SCOPE;

        glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
        for (unsigned int i = 0; i < _textures.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, _textures[i]);
        }
    }

	void GBuffer::getTextureContent(GBuffer::Texture texture, void * content) const noexcept {
		KREOGL_PROFILING_SCOPE;

		// Profiling results:
		// 		glReadPixels: 1ms
		//		glGetTexImage: 3.5ms
		glBindFramebuffer(GL_READ_FRAMEBUFFER, _frameBuffer);
		glReadBuffer(GL_COLOR_ATTACHMENT0 + int(texture));
		glReadPixels(0, 0, _size.x, _size.y, GL_RGBA, GL_FLOAT, content);
	}

	glm::vec4 GBuffer::readPixel(GBuffer::Texture texture, const glm::ivec2 & pixel) const noexcept {
		KREOGL_PROFILING_SCOPE;

		assert(pixel.x >= 0 && pixel.x < _size.x && pixel.y >= 0 && pixel.y < _size.y);

		static std::vector<glm::vec4> pixels;
		pixels.resize(_size.x * _size.y);
		getTextureContent(texture, pixels.data());

		const auto pixelIndex = pixel.x + (_size.y - pixel.y) * _size.x;
		return pixels[pixelIndex];
	}
}