#include "Viewport.hpp"

#include <cassert>

#include "kreogl/impl/RAII/ScopedGLFeature.hpp"
#include "kreogl/impl/shaders/ShaderPipeline.hpp"

namespace kreogl {
    Viewport::Viewport(const ConstructionParams & params) noexcept
        : _resolution(params.resolution)
        , _onScreenPosition(params.onScreenPosition)
        , _onScreenSize(params.onScreenSize)
        , _zOrder(params.zOrder)
        , _gbuffer(params.resolution)
    {
        glBindTexture(GL_TEXTURE_2D, _renderTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _resolution.x, _resolution.y, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTexture, 0);

        glBindTexture(GL_TEXTURE_2D, _depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _resolution.x, _resolution.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTexture, 0);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }

    void Viewport::draw(const DrawParams & params) const noexcept {
        fillGBuffer(params);
        renderToTexture(params);
    }

    void Viewport::fillGBuffer(const DrawParams & params) const noexcept {
        _gbuffer.bindForWriting();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const ScopedGLFeature depth(GL_DEPTH_TEST);
        params.shaderPipeline.runShaders(ShaderStep::GBuffer, params);
    }

    void Viewport::renderToTexture(const DrawParams & params) const noexcept {
        _gbuffer.bindForReading();

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBuffer);
        glClear(GL_COLOR_BUFFER_BIT);
        glBlitFramebuffer(0, 0, _resolution.x, _resolution.y, 0, 0, _resolution.x, _resolution.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        params.shaderPipeline.runShaders(ShaderStep::Lighting, params);
        params.shaderPipeline.runShaders(ShaderStep::PostLighting, params);
        params.shaderPipeline.runShaders(ShaderStep::PostProcess, params);
    }

    void Viewport::setResolution(const glm::ivec2 &resolution) noexcept {
        _resolution = resolution;
        _gbuffer.resize(_resolution);

        glBindTexture(GL_TEXTURE_2D, _renderTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _resolution.x, _resolution.y, 0, GL_RGBA, GL_FLOAT, nullptr);
    }
}