#include "DirectionalLightShader.hpp"
#include "impl/shaders/Quad/QuadGLSL.hpp"

#include "impl/kreogl.hpp"
#include "impl/GBuffer.hpp"
#include "impl/RAII/ScopedBindFramebuffer.hpp"
#include "impl/RAII/ScopedGLFeature.hpp"
#include "Camera.hpp"
#include "World.hpp"
#include "shapes/Quad.hpp"

namespace kreogl {
    DirectionalLightShader::DirectionalLightShader() noexcept {
        init();

        use();
        for (size_t i = 0; i < KREOGL_MAX_CSM_COUNT; ++i) {
            _csmGLSL.shadowMap[i] = GBuffer::nbAttributes + i;
        }
    }

    void DirectionalLightShader::addSourceFiles() noexcept {
        addSourceFile(QuadGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(DirectionalLightGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(CascadedShadowMapGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> DirectionalLightShader::getUniforms() noexcept {
        std::vector<UniformBase *> ret {
            // DirectionalLightGLSL
            &_glsl.viewPos,
            &_glsl.screenSize,
            &_glsl.color,
            &_glsl.direction,
            &_glsl.ambientStrength,
            &_glsl.diffuseStrength,
            &_glsl.specularStrength,
            // CascadedShadowMapGLSL
            &_csmGLSL.cascadeCount,
            &_csmGLSL.bias,
            &_csmGLSL.pcfSamples,
            &_csmGLSL.proj,
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
        use();

        const ScopedGLFeature blend(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        _csmGLSL.proj = params.camera.getProjMatrix();
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
            }

            kreogl::fillShadowMap(*light, params);
            kreogl::runShaders(ShaderStep::ShadowMap, params);

            use();
            _glsl.color = light->color;
            _glsl.direction = light->direction;
            _glsl.ambientStrength = light->ambientStrength;
            _glsl.diffuseStrength = light->diffuseStrength;
            _glsl.specularStrength = light->specularStrength;
            _csmGLSL.pcfSamples = light->shadowPCFSamples;
            _csmGLSL.bias = light->shadowMapBias;

            for (size_t i = 0; i < light->cascadeEnds.size(); ++i)
                _csmGLSL.cascadeEnd[i] = light->cascadeEnds[i];

            _csmGLSL.cascadeCount = (int)light->cascadeEnds.size();

            for (size_t i = 0; i < light->cascadeEnds.size(); ++i) {
                glActiveTexture((GLenum)(GL_TEXTURE0 + GBuffer::nbAttributes + i));
                glBindTexture(GL_TEXTURE_2D, light->cascadedShadowMap.textures[i]);
                _csmGLSL.lightSpaceMatrix[i] = light->getCascadedShadowMapLightSpaceMatrix(params, i);
            }

            kreogl::shapes::drawQuad();
        }
    }
}