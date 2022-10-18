#include "VolumetricPointLightShader.hpp"
#include "kreogl/impl/shaders/helpers/Quad/QuadGLSL.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/impl/GBuffer.hpp"
#include "kreogl/impl/RAII/ScopedGLFeature.hpp"
#include "kreogl/impl/shapes/Quad.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    VolumetricPointLightShader::VolumetricPointLightShader() noexcept {
        KREOGL_PROFILING_SCOPE;

        init("VolumetricPointLightShader");

        useWithoutUniformCheck();

        _glsl.gposition = (int)GBuffer::Texture::Position;
        _glsl.gdiffuse = (int)GBuffer::Texture::DiffuseAndShouldIgnoreLighting;
        _shadowCubeGLSL.shadowMap = (int)GBuffer::Texture::Count;

#ifndef NDEBUG
        assert(_shadowCubeGLSL.viewPos.location == _glsl.viewPos.location);
        _shadowCubeGLSL.viewPos.used = true;

        assert(_shadowCubeGLSL.position.location == _pointLightGLSL.position.location);
        _shadowCubeGLSL.position.used = true;
#endif
    }

    void VolumetricPointLightShader::addSourceFiles() noexcept {
        KREOGL_PROFILING_SCOPE;

        addSourceFile(QuadGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(VolumetricLightingGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(PointLightGLSL::GetDirection::frag, GL_FRAGMENT_SHADER);
        addSourceFile(SampleShadowCubeGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> VolumetricPointLightShader::getUniforms() noexcept {
        KREOGL_PROFILING_SCOPE;

        return {
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
            // PointLightGLSL::GetDirection
            &_pointLightGLSL.position,
            // SampleShadowCubeGLSL
            &_shadowCubeGLSL.shadowMap,
            &_shadowCubeGLSL.position,
            &_shadowCubeGLSL.viewPos,
            &_shadowCubeGLSL.farPlane,
            // Unused
            // &_shadowCubeGLSL.minBias,
            // &_shadowCubeGLSL.maxBias,
        };
    }

    void VolumetricPointLightShader::draw(const DrawParams & params) noexcept {
        KREOGL_PROFILING_SCOPE;

        auto uniformChecker = use(false);

        const ScopedGLFeature blend(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        glActiveTexture(GL_TEXTURE0 + (int)GBuffer::Texture::Count);

        _glsl.inverseView = glm::inverse(params.camera.getViewMatrix());
        _glsl.inverseProj = glm::inverse(params.camera.getProjMatrix());
        _glsl.viewPos = params.camera.getPosition();
        _glsl.screenSize = params.camera.getViewport().getResolution();

        for (const auto light : params.world.getPointLights()) {
            if (light->volumetricLighting == std::nullopt)
                continue;

            _glsl.scattering = light->volumetricLighting->scattering;
            _glsl.nbSteps = light->volumetricLighting->nbSteps;
            _glsl.defaultStepLength = light->volumetricLighting->defaultStepLength;
            _glsl.intensity = light->volumetricLighting->intensity;

            _glsl.color = light->color;
            _pointLightGLSL.position = light->position;

            glBindTexture(GL_TEXTURE_CUBE_MAP, light->shadowCube.texture);
            _shadowCubeGLSL.farPlane = light->getRadius();

            uniformChecker.shouldCheck = true;
            kreogl::shapes::drawQuad();
        }
    }
}