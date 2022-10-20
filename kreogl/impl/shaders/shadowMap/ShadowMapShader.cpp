#include "ShadowMapShader.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/impl/RAII/ScopedBindFramebuffer.hpp"
#include "kreogl/impl/RAII/ScopedGLFeature.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    template<typename T, typename Func>
    void ShadowMapShader::drawImpl(T & depthMap, Func && draw, const DrawParams & params) noexcept {
        KREOGL_PROFILING_SCOPE;

        glViewport(0, 0, depthMap.getSize(), depthMap.getSize());
        glCullFace(GL_FRONT);

        const ScopedBindFramebuffer bound(depthMap.frameBuffer);
        const ScopedGLFeature depth(GL_DEPTH_TEST);

        useWithoutUniformCheck(); // Would need to pass the checker down to drawToTexture

        draw();

        glCullFace(GL_BACK);

        const auto & viewport = params.camera.getViewport();
        const auto & resolution = viewport.getResolution();
        glViewport(0, 0, resolution.x, resolution.y);
    }

    void ShadowMapShader::draw(const DirectionalLight & light, const DrawParams & params) noexcept {
        KREOGL_PROFILING_SCOPE;

        light.cascadedShadowMap.setSize(light.shadowMapSize);

        drawImpl(light.cascadedShadowMap, [&]() noexcept {
            for (size_t i = 0; i < light.cascadeEnds.size(); ++i) {
                const float cascadeStart = (i == 0 ? KREOGL_SHADOW_MAP_NEAR_PLANE : light.cascadeEnds[i - 1]);
                const float cascadeEnd = light.cascadeEnds[i];
                if (cascadeStart >= cascadeEnd)
                    continue;
                const auto & texture = light.cascadedShadowMap.textures[i];
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
                drawToTexture(texture, light.getLightSpaceMatrixForCascade(params, i), params);
            }
        }, params);
    }

    void ShadowMapShader::draw(const SpotLight & light, const DrawParams & params) noexcept {
        KREOGL_PROFILING_SCOPE;

        light.shadowMap.setSize(light.shadowMapSize);

        drawImpl(light.shadowMap, [&]() noexcept {
            drawToTexture(light.shadowMap.texture, light.getLightSpaceMatrix(), params);
        }, params);
    }
}