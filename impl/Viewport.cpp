#include "Viewport.hpp"

#include <cassert>

#include "kreogl.hpp"
#include "impl/RAII/ScopedGLFeature.hpp"
#include "impl/shaders/ShaderStep.hpp"

namespace kreogl {
    Viewport::Viewport(const ConstructionParams & params) noexcept
        : _resolution(params.resolution)
        , _onScreenPosition(params.onScreenPosition)
        , _onScreenSize(params.onScreenSize)
        , _zOrder(params.zOrder)
        , _gbuffer(params.resolution)
    {
        init();
    }

    void Viewport::init() noexcept {
        glBindTexture(GL_TEXTURE_2D, _renderTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _resolution.x, _resolution.y, 0, GL_RGBA, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _renderTexture, 0);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    }

    void Viewport::draw(const DrawParams & params) noexcept {
        fillGBuffer(params);
        renderToTexture(params);
    }

    void Viewport::fillGBuffer(const DrawParams & params) noexcept {
        _gbuffer.bindForWriting();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const ScopedGLFeature depth(GL_DEPTH_TEST);
        runShaders(ShaderStep::GBuffer, params);
    }

    void Viewport::renderToTexture(const DrawParams & params) noexcept {
        _gbuffer.bindForReading();

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _frameBuffer);
        glClear(GL_COLOR_BUFFER_BIT);

        glBlitFramebuffer(0, 0, _resolution.x, _resolution.y, 0, 0, _resolution.x, _resolution.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        runShaders(ShaderStep::Lighting, params);
        runShaders(ShaderStep::PostLighting, params);
        runShaders(ShaderStep::PostProcess, params);
    }

    void Viewport::setResolution(const glm::ivec2 &resolution) noexcept {
        _resolution = resolution;
        _gbuffer.resize(_resolution);
        // need to regen render texture
        init();
    }
}