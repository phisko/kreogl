#include "DirectionalLightShader.hpp"
#include "kreogl/impl/shaders/helpers/Quad/QuadGLSL.hpp"

#include "kreogl/impl/kreogl.hpp"
#include "kreogl/impl/GBuffer.hpp"
#include "kreogl/impl/RAII/ScopedBindFramebuffer.hpp"
#include "kreogl/impl/RAII/ScopedGLFeature.hpp"
#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/shapes/Quad.hpp"

namespace kreogl {
    DirectionalLightShader::DirectionalLightShader() noexcept {
        init();

        useWithoutUniformCheck();

        _glsl.gposition = (int)GBuffer::Texture::Position;
        _glsl.gnormal = (int)GBuffer::Texture::Normal;
        _glsl.gdiffuse = (int)GBuffer::Texture::DiffuseAndShouldIgnoreLighting;
        _glsl.gspecular = (int)GBuffer::Texture::Specular;

        for (size_t i = 0; i < KREOGL_MAX_CSM_COUNT; ++i)
            _csmGLSL.shadowMap[i] = (int)GBuffer::Texture::Count + i;
    }

    void DirectionalLightShader::addSourceFiles() noexcept {
        addSourceFile(QuadGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(DirectionalLightGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(SampleCascadedShadowMapGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> DirectionalLightShader::getUniforms() noexcept {
        std::vector<UniformBase *> ret {
            // DirectionalLightGLSL
            &_glsl.gposition,
            &_glsl.gnormal,
            &_glsl.gdiffuse,
            &_glsl.gspecular,
            &_glsl.viewPos,
            &_glsl.screenSize,
            &_glsl.color,
            &_glsl.direction,
            &_glsl.ambientStrength,
            &_glsl.diffuseStrength,
            &_glsl.specularStrength,
            // SampleCascadedShadowMapGLSL
            &_csmGLSL.cascadeCount,
            &_csmGLSL.minBias,
            &_csmGLSL.maxBias,
            &_csmGLSL.pcfSamples,
            &_csmGLSL.view
        };

        for (auto & uniform : _csmGLSL.shadowMap)
            ret.push_back(&uniform);

        for (auto & uniform : _csmGLSL.lightSpaceMatrix)
            ret.push_back(&uniform);

        for (auto & uniform : _csmGLSL.cascadeEnd)
            ret.push_back(&uniform);

        return ret;
    }

    void DirectionalLightShader::draw(const DrawParams &params) noexcept {
        const auto uniformChecker = use();

        const ScopedGLFeature blend(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        _csmGLSL.view = params.camera.getViewMatrix();

        _glsl.viewPos = params.camera.getPosition();
        _glsl.screenSize = params.camera.getViewport().getResolution();

        for (const auto light : params.world.getDirectionalLights()) {
            if (light->castShadows) {
                const ScopedBindFramebuffer bind(light->cascadedShadowMap.frameBuffer);
                for (const auto & texture : light->cascadedShadowMap.textures) {
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
                    glClear(GL_DEPTH_BUFFER_BIT);
                }
                kreogl::fillShadowMap(*light, params);
            }

            const auto uniformChecker = use();
            _glsl.color = light->color;
            _glsl.direction = light->direction;
            _glsl.ambientStrength = light->ambientStrength;
            _glsl.diffuseStrength = light->diffuseStrength;
            _glsl.specularStrength = light->specularStrength;
            _csmGLSL.pcfSamples = light->shadowPCFSamples;
            _csmGLSL.maxBias = light->shadowMapMaxBias;
            _csmGLSL.minBias = light->shadowMapMinBias;

            for (size_t i = 0; i < light->cascadeEnds.size(); ++i) {
                glActiveTexture((GLenum)(GL_TEXTURE0 + (int)GBuffer::Texture::Count + i));
                glBindTexture(GL_TEXTURE_2D, light->cascadedShadowMap.textures[i]);
                _csmGLSL.lightSpaceMatrix[i] = light->getLightSpaceMatrixForCascade(params, i);
                _csmGLSL.cascadeEnd[i] = light->cascadeEnds[i];
            }
            _csmGLSL.cascadeCount = (int)light->cascadeEnds.size();

            kreogl::shapes::drawQuad();
        }
    }
}