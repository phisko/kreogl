#include "PositionShadowMapShader.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"

namespace kreogl {
    PositionShadowMapShader::PositionShadowMapShader() noexcept {
        init();

        use();
        _glsl.proj = glm::mat4(1.f);
    }

    void PositionShadowMapShader::addSourceFiles() noexcept {
        addSourceFile(PositionProjViewModelGLSL::vert, GL_VERTEX_SHADER);
    }

    std::vector<UniformBase *> PositionShadowMapShader::getUniforms() noexcept {
        return {
            &_glsl.proj,
            &_glsl.view,
            &_glsl.model
        };
    }

    void PositionShadowMapShader::drawToTexture(GLuint texture, const glm::mat4 & lightSpaceMatrix, const DrawParams & params) noexcept {
        use();

		_glsl.view = lightSpaceMatrix;

        for (const auto object : params.world.getObjects(VertexSpecification::positionColor)) {
            _glsl.model = object->transform;
            object->model->draw();
        }
	}
}
