#include "VolumetricDirectionalLightShader.hpp"
#include "kreogl/impl/shaders/helpers/Quad/QuadGLSL.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/impl/GBuffer.hpp"
#include "kreogl/impl/RAII/ScopedGLFeature.hpp"
#include "kreogl/impl/shapes/Quad.hpp"

namespace kreogl {
    VolumetricDirectionalLightShader::VolumetricDirectionalLightShader() noexcept {
        init("VolumetricDirectionalLightShader");

        useWithoutUniformCheck();

        _glsl.gposition = (int)GBuffer::Texture::Position;
        _glsl.gdiffuse = (int)GBuffer::Texture::DiffuseAndShouldIgnoreLighting;

        for (size_t i = 0; i < KREOGL_MAX_CSM_COUNT; ++i)
            _csmGLSL.shadowMap[i] = (int)GBuffer::Texture::Count + i;
    }

    void VolumetricDirectionalLightShader::addSourceFiles() noexcept {
        addSourceFile(QuadGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(VolumetricLightingGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(DirectionalLightGLSL::GetDirection::frag, GL_FRAGMENT_SHADER);
        addSourceFile(SampleCascadedShadowMapGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> VolumetricDirectionalLightShader::getUniforms() noexcept {
        std::vector<UniformBase *> ret {
            // VolumetricLightingGLSL
            &_glsl.gposition,
            &_glsl.gdiffuse,
            &_glsl.scattering,
            &_glsl.nbSteps,
            &_glsl.defaultStepLength,
            &_glsl.intensity,
            &_glsl.inverseView,
            &_glsl.inverseProj,
            &_glsl.viewPos,
            &_glsl.screenSize,
            &_glsl.color,
            // DirectionalLightGLSL::GetDirection
            &_dirLightGLSL.direction,
            // SampleCascadedShadowMapGLSL
            &_csmGLSL.cascadeCount,
            &_csmGLSL.view,
            // Unused
            // &_csmGLSL.minBias,
            // &_csmGLSL.maxBias,
            // &_csmGLSL.pcfSamples,
        };

        for (auto & uniform : _csmGLSL.shadowMap)
            ret.push_back(&uniform);

        for (auto & uniform : _csmGLSL.lightSpaceMatrix)
            ret.push_back(&uniform);

        for (auto & uniform : _csmGLSL.cascadeEnd)
            ret.push_back(&uniform);

        return ret;
    }

    void VolumetricDirectionalLightShader::draw(const DrawParams & params) noexcept {
        auto uniformChecker = use(false);

        const ScopedGLFeature blend(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        _csmGLSL.view = params.camera.getViewMatrix();
        _glsl.inverseView = glm::inverse(params.camera.getViewMatrix());
        _glsl.inverseProj = glm::inverse(params.camera.getProjMatrix());
        _glsl.viewPos = params.camera.getPosition();
        _glsl.screenSize = params.camera.getViewport().getResolution();

        for (const auto light : params.world.getDirectionalLights()) {
            if (light->volumetricLighting == std::nullopt)
                continue;

            _glsl.scattering = light->volumetricLighting->scattering;
            _glsl.nbSteps = light->volumetricLighting->nbSteps;
            _glsl.defaultStepLength = light->volumetricLighting->defaultStepLength;
            _glsl.intensity = light->volumetricLighting->intensity;

            _glsl.color = light->color;
            _dirLightGLSL.direction = light->direction;

            for (size_t i = 0; i < light->cascadeEnds.size(); ++i) {
                glActiveTexture((GLenum)(GL_TEXTURE0 + (int)GBuffer::Texture::Count + i));
                glBindTexture(GL_TEXTURE_2D, light->cascadedShadowMap.textures[i]);
                _csmGLSL.lightSpaceMatrix[i] = light->getLightSpaceMatrixForCascade(params, i);
                _csmGLSL.cascadeEnd[i] = light->cascadeEnds[i];
            }
            _csmGLSL.cascadeCount = (int)light->cascadeEnds.size();

            uniformChecker.shouldCheck = true;
            kreogl::shapes::drawQuad();
        }
    }
}