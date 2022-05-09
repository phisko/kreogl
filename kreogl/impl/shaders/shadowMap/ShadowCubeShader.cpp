#include "ShadowCubeShader.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/impl/RAII/ScopedBindFramebuffer.hpp"
#include "kreogl/impl/RAII/ScopedGLFeature.hpp"

namespace kreogl {
    std::vector<UniformBase *> ShadowCubeShader::getUniforms() noexcept {
        std::vector<UniformBase *> ret {
            &_glsl.lightPos,
            &_glsl.farPlane
        };

        for (auto & uniform : _glsl.lightSpaceMatrices)
            ret.push_back(&uniform);

        return ret;
    }

    void ShadowCubeShader::addSourceFiles() noexcept {
        addSourceFile(ShadowCubeGLSL::geom, GL_GEOMETRY_SHADER);
        addSourceFile(ShadowCubeGLSL::frag, GL_FRAGMENT_SHADER);
    }

    void ShadowCubeShader::draw(const PointLight & light, const DrawParams & params) noexcept {
        light.shadowCube.setSize(light.shadowMapSize);

        glViewport(0, 0, light.shadowCube.getSize(), light.shadowCube.getSize());
        glCullFace(GL_FRONT);

        const ScopedBindFramebuffer bound(light.shadowCube.frameBuffer);
        const ScopedGLFeature depth(GL_DEPTH_TEST);

        use();

        static struct {
            glm::vec3 target;
            glm::vec3 up;
        } directions[] = {
            { { 1.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
            { { -1.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
            { { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
            { { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
            { { 0.0f, 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } },
            { { 0.0f, 0.0f, -1.0f }, { 0.0f, -1.0f, 0.0f } }
        };

        const auto radius = light.getRadius();
        const auto proj = glm::perspective(glm::radians(90.f), (float)light.shadowMapSize / (float)light.shadowMapSize, KREOGL_SHADOW_MAP_NEAR_PLANE, radius);
        for (unsigned int i = 0; i < 6; ++i) {
            const auto view = glm::lookAt(light.position, light.position + directions[i].target, directions[i].up);
            _glsl.lightSpaceMatrices[i] = proj * view;
        }

        _glsl.lightPos = light.position;
        _glsl.farPlane = radius;

        drawObjects(params);

        const auto & viewport = params.camera.getViewport();
        const auto & resolution = viewport.getResolution();
        glViewport(0, 0, resolution.x, resolution.y);
        glCullFace(GL_BACK);
    }
}