#include "DirectionalLightShader.hpp"
#include "kreogl/impl/shaders/helpers/Quad/QuadGLSL.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/impl/GBuffer.hpp"
#include "kreogl/impl/RAII/ScopedBindFramebuffer.hpp"
#include "kreogl/impl/RAII/ScopedGLFeature.hpp"
#include "kreogl/impl/shaders/ShaderPipeline.hpp"
#include "kreogl/impl/shaders/shadowMap/ShadowMapShader.hpp"
#include "kreogl/impl/shapes/Quad.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    DirectionalLightShader::DirectionalLightShader() noexcept {
        KREOGL_PROFILING_SCOPE;

        init("DirectionalLightShader");

        useWithoutUniformCheck();

        _glsl.gposition = (int)GBuffer::Texture::Position;
        _glsl.gnormal = (int)GBuffer::Texture::Normal;
        _glsl.gdiffuse = (int)GBuffer::Texture::DiffuseAndShouldIgnoreLighting;
        _glsl.gspecular = (int)GBuffer::Texture::Specular;

        for (size_t i = 0; i < KREOGL_MAX_CSM_COUNT; ++i)
            _csmGLSL.shadowMap[i] = (int)GBuffer::Texture::Count + i;
    }

    void DirectionalLightShader::addSourceFiles() noexcept {
        KREOGL_PROFILING_SCOPE;

        addSourceFile(QuadGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(DirectionalLightGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(SampleCascadedShadowMapGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> DirectionalLightShader::getUniforms() noexcept {
        KREOGL_PROFILING_SCOPE;

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
        KREOGL_PROFILING_SCOPE;

        useWithoutUniformCheck();

        const ScopedGLFeature blend(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        _csmGLSL.view = params.camera.getViewMatrix();

        _glsl.viewPos = params.camera.getPosition();
        _glsl.screenSize = params.camera.getViewport().getResolution();

        for (const auto light : params.world.getDirectionalLights()) {
            if (light->castShadows)
                updateShadowMap(*light, params);

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

    void DirectionalLightShader::updateShadowMap(const DirectionalLight & light, const DrawParams & params) noexcept {
        KREOGL_PROFILING_SCOPE;

        const ScopedBindFramebuffer bind(light.cascadedShadowMap.frameBuffer);
        for (const auto & texture : light.cascadedShadowMap.textures) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
            glClear(GL_DEPTH_BUFFER_BIT);
        }

        const auto shaders = params.shaderPipeline.getShaders(ShaderStep::ShadowMap);
        if (!shaders)
            return;

        for (const auto shader : *shaders) {
            const auto shadowMapShader = static_cast<ShadowMapShader *>(shader);
            shadowMapShader->draw(light, params);
        }
    }
}