#include "PositionShadowCubeShader.hpp"
#include "kreogl/impl/shaders/VertexSpecification.hpp"
#include "kreogl/World.hpp"

namespace kreogl {
    PositionShadowCubeShader::PositionShadowCubeShader() noexcept {
        init();
        useWithoutUniformCheck();
        _glsl.proj = glm::mat4(1.f);
        _glsl.view = glm::mat4(1.f);
    }

    void PositionShadowCubeShader::addSourceFiles() noexcept {
        ShadowCubeShader::addSourceFiles();
        addSourceFile(PositionProjViewModelGLSL::vert, GL_VERTEX_SHADER);
    }

    std::vector<UniformBase *> PositionShadowCubeShader::getUniforms() noexcept {
        auto uniforms = ShadowCubeShader::getUniforms();
        uniforms.push_back(&_glsl.proj);
        uniforms.push_back(&_glsl.view);
        uniforms.push_back(&_glsl.model);
        return uniforms;
    }

    void PositionShadowCubeShader::drawObjects(const DrawParams & params) noexcept {
        for (const auto object : params.world.getObjects(VertexSpecification::positionColor))
            if (object->castShadows) {
                _glsl.model = object->transform;
                object->model->draw();
            }
    }
}