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
        _pbos.resize((int)Texture::Count);

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

        for (const auto & pbo : _pbos) {
            if (!pbo.init)
                continue;
            const auto pixelSize = _size.x * _size.y * sizeof(float) * 4 /*GL_RGBA32F*/;
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo.forRead);
            glBufferData(GL_PIXEL_PACK_BUFFER, pixelSize, 0, GL_STREAM_READ);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo.forWrite);
            glBufferData(GL_PIXEL_PACK_BUFFER, pixelSize, 0, GL_STREAM_READ);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        }
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
            _pbos[i].upToDate = false;
        }
    }

    GBuffer::MappedTexture::MappedTexture(GLuint pbo) noexcept : _pbo(pbo) {
        KREOGL_PROFILING_SCOPE;

        glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
        _data = (const float *)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
    }

    GBuffer::MappedTexture::~MappedTexture() noexcept {
        KREOGL_PROFILING_SCOPE;

        glBindBuffer(GL_PIXEL_PACK_BUFFER, _pbo);
        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }

    GBuffer::MappedTexture GBuffer::getMappedTexture(Texture texture) noexcept {
        KREOGL_PROFILING_SCOPE;

        auto & pbo = _pbos[int(texture)];
        if (!pbo.init) {
            pbo.init = true;

            const auto size = _size.x * _size.y * sizeof(float) * 4 /*GL_RGBA*/;
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo.forRead);
            glBufferData(GL_PIXEL_PACK_BUFFER, GLsizeiptr(size), nullptr, GL_STREAM_READ);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo.forWrite);
            glBufferData(GL_PIXEL_PACK_BUFFER, GLsizeiptr(size), nullptr, GL_STREAM_READ);
        }

        if (!pbo.upToDate) {
            pbo.upToDate = true;

            std::swap(pbo.forRead.get(), pbo.forWrite.get());

            glBindTexture(GL_TEXTURE_2D, _textures[int(texture)]);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo.forWrite);
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, nullptr);
        }

        return MappedTexture{ pbo.forRead };
    }
}