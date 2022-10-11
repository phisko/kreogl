#include "PointLightShader.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/shapes/Sphere.hpp"
#include "kreogl/impl/GBuffer.hpp"
#include "kreogl/impl/RAII/ScopedBindFramebuffer.hpp"
#include "kreogl/impl/RAII/ScopedGLFeature.hpp"
#include "kreogl/impl/shaders/ShaderPipeline.hpp"
#include "kreogl/impl/shaders/shadowMap/ShadowCubeShader.hpp"

namespace kreogl {
    PointLightShader::PointLightShader() noexcept {
        init("PointLightShader");

#ifndef NDEBUG
        assert(_glsl.position.location == _shadowCubeGLSL.position.location);
        _shadowCubeGLSL.position.used = true;

        assert(_glsl.viewPos.location == _shadowCubeGLSL.viewPos.location);
        _shadowCubeGLSL.viewPos.used = true;
#endif

        useWithoutUniformCheck();

        _glsl.gposition = (int)GBuffer::Texture::Position;
        _glsl.gnormal = (int)GBuffer::Texture::Normal;
        _glsl.gdiffuse = (int)GBuffer::Texture::DiffuseAndShouldIgnoreLighting;
        _glsl.gspecular = (int)GBuffer::Texture::Specular;

        _shadowCubeGLSL.shadowMap = (int)GBuffer::Texture::Count;
    }

    void PointLightShader::addSourceFiles() noexcept {
        addSourceFile(PositionProjViewModelGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(PointLightGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(SampleShadowCubeGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> PointLightShader::getUniforms() noexcept {
        return {
            // PositionProjViewModelGLSL
            &_ppvmGLSL.proj,
            &_ppvmGLSL.view,
            &_ppvmGLSL.model,
            // PointLightGLSL
            &_glsl.gposition,
            &_glsl.gnormal,
            &_glsl.gdiffuse,
            &_glsl.gspecular,
            &_glsl.viewPos,
            &_glsl.screenSize,
            &_glsl.color,
            &_glsl.position,
            &_glsl.diffuseStrength,
            &_glsl.specularStrength,
            &_glsl.attenuationConstant,
            &_glsl.attenuationLinear,
            &_glsl.attenuationQuadratic,
            // SampleShadowCubeGLSL
            &_shadowCubeGLSL.shadowMap,
            &_shadowCubeGLSL.position,
            &_shadowCubeGLSL.viewPos,
            &_shadowCubeGLSL.farPlane,
            &_shadowCubeGLSL.minBias,
            &_shadowCubeGLSL.maxBias,
        };
    }

    void PointLightShader::draw(const DrawParams &params) noexcept {
        useWithoutUniformCheck();

        const ScopedGLFeature cull(GL_CULL_FACE);
        const ScopedGLFeature blend(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);

        _glsl.screenSize = params.camera.getViewport().getResolution();
        _glsl.viewPos = params.camera.getPosition();
        assert(_shadowCubeGLSL.viewPos.location == _glsl.viewPos.location); // If this fails, we need to explicitly set both uniforms

        glActiveTexture((GLenum)(GL_TEXTURE0 + (int)GBuffer::Texture::Count));

        for (const auto light : params.world.getPointLights()) {
            if (light->castShadows)
                updateShadowMap(*light, params);

            const auto radius = light->getRadius();
            if (glm::length(params.camera.getPosition() - light->position) < radius)
                glCullFace(GL_BACK);
            else
                glCullFace(GL_FRONT);

            const auto uniformChecker = use();

            _ppvmGLSL.proj = params.camera.getProjMatrix();
            _ppvmGLSL.view = params.camera.getViewMatrix();

            glm::mat4 model(1.f);
            model = glm::translate(model, light->position);
            model = glm::scale(model, glm::vec3(radius));
            _ppvmGLSL.model = model;

            _glsl.color = light->color;
            _glsl.position = light->position;
            _glsl.diffuseStrength = light->diffuseStrength;
            _glsl.specularStrength = light->specularStrength;
            _glsl.attenuationConstant = light->attenuationConstant;
            _glsl.attenuationLinear = light->attenuationLinear;
            _glsl.attenuationQuadratic = light->attenuationQuadratic;
            _shadowCubeGLSL.farPlane = light->getRadius();
            _shadowCubeGLSL.minBias = light->shadowMapMinBias;
            _shadowCubeGLSL.maxBias = light->shadowMapMaxBias;

            glBindTexture(GL_TEXTURE_CUBE_MAP, light->shadowCube.texture);
            kreogl::shapes::drawSphere();
        }

        glCullFace(GL_BACK);
    }

    void PointLightShader::updateShadowMap(const PointLight & light, const DrawParams & params) noexcept {
        const ScopedBindFramebuffer bind(light.shadowCube.frameBuffer);
        glClear(GL_DEPTH_BUFFER_BIT);

        const auto shaders = params.shaderPipeline.getShaders(ShaderStep::ShadowCube);
        if (!shaders)
            return;

        for (const auto shader : *shaders) {
            const auto shadowCubeShader = static_cast<ShadowCubeShader *>(shader);
            shadowCubeShader->draw(light, params);
        }
    }
}