#include "PositionEmptyNormalColorShader.hpp"
#include "impl/shaders/ApplyTransparency/ApplyTransparencyGLSL.hpp"
#include "impl/kreogl.hpp"

#include "Camera.hpp"
#include "World.hpp"

namespace kreogl {
    PositionEmptyNormalColorShader::PositionEmptyNormalColorShader() noexcept {
        init();
    }

    void PositionEmptyNormalColorShader::addSourceFiles() noexcept {
        addSourceFile(PositionEmptyNormalColorGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(PositionEmptyNormalColorGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(ApplyTransparencyGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> PositionEmptyNormalColorShader::getUniforms() noexcept {
        return {
            &_glsl.proj,
            &_glsl.view,
            &_glsl.model,
            &_glsl.viewPos,
            &_glsl.color,
            &_glsl.userData
        };
    }

	void PositionEmptyNormalColorShader::draw(const DrawParams & params) noexcept {
		use();

		_glsl.view = params.camera.getViewMatrix();
		_glsl.proj = params.camera.getProjMatrix();
        _glsl.viewPos = params.camera.getPosition();

        for (const auto object : params.world.getObjects(VertexSpecification::PositionEmptyNormalColor)) {
            _glsl.model = object->transform;
            _glsl.color = object->color;
            _glsl.userData = object->userData;
            for (const auto & mesh : object->model->meshes) {
                mesh.draw();
            }
        }
	}
}
