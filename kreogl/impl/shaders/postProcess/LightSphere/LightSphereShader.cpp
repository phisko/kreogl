#include "LightSphereShader.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/impl/shapes/Sphere.hpp"
#include "kreogl/impl/RAII/ScopedGLFeature.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    LightSphereShader::LightSphereShader() noexcept {
        KREOGL_PROFILING_SCOPE;

        init("LightSphereShader");
    }

    void LightSphereShader::addSourceFiles() noexcept {
        KREOGL_PROFILING_SCOPE;

        addSourceFile(PositionProjViewModelGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(ColorGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> LightSphereShader::getUniforms() noexcept {
        KREOGL_PROFILING_SCOPE;

        return {
            &_glsl.proj,
            &_glsl.view,
            &_glsl.model,
            &_colorGLSL.color
        };
    }

    void LightSphereShader::draw(const DrawParams & params) noexcept {
        KREOGL_PROFILING_SCOPE;

        const ScopedGLFeature depth(GL_DEPTH_TEST);
        const ScopedGLFeature cull(GL_CULL_FACE);

        // Not sure why we need to cull the front face here, logic would suggest the back one
        // Perhaps we're drawing the sphere "backwards"?
        glCullFace(GL_FRONT);

        const auto uniformChecker = use();

        _glsl.proj = params.camera.getProjMatrix();
        _glsl.view = params.camera.getViewMatrix();

        for (const auto light : params.world.getDirectionalLights()) {
            const auto pos = params.camera.getPosition() - light->direction * light->lightSphereDistance;
            drawLight(*light, pos);
        }

        for (const auto light : params.world.getPointLights())
            drawLight(*light, light->position);

        for (const auto light : params.world.getSpotLights()) {
            const auto lightToCamera = light->position - params.camera.getPosition();
            const bool towardsCamera = glm::dot(lightToCamera, light->direction) < 0;
            if (towardsCamera)
                drawLight(*light, light->position);
        }

        glCullFace(GL_BACK);
    }

    void LightSphereShader::drawLight(const Light & light, const glm::vec3 & pos) noexcept {
        KREOGL_PROFILING_SCOPE;

        glm::mat4 model{ 1.f };
        model = glm::translate(model, pos);
        model = glm::scale(model, glm::vec3(light.lightSphereSize));

        _glsl.model = model;
        _colorGLSL.color = light.color;

        shapes::drawSphere();
    }
}