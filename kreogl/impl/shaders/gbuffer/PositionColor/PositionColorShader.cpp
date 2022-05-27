#include "PositionColorShader.hpp"
#include "kreogl/impl/shaders/helpers/ApplyTransparency/ApplyTransparencyGLSL.hpp"
#include "kreogl/impl/kreogl.hpp"

#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"

namespace kreogl {
    PositionColorShader::PositionColorShader() noexcept {
        init("PositionColorShader");
    }

    void PositionColorShader::addSourceFiles() noexcept {
        addSourceFile(PositionColorGLSL::vert, GL_VERTEX_SHADER);
        addSourceFile(PositionColorGLSL::frag, GL_FRAGMENT_SHADER);
        addSourceFile(ApplyTransparencyGLSL::frag, GL_FRAGMENT_SHADER);
    }

    std::vector<UniformBase *> PositionColorShader::getUniforms() noexcept {
        return {
            &_glsl.proj,
            &_glsl.view,
            &_glsl.model,
            &_glsl.viewPos,
            &_glsl.color,
            &_glsl.userData
        };
    }

	void PositionColorShader::draw(const DrawParams & params) noexcept {
		auto uniformChecker = use(false);

		_glsl.view = params.camera.getViewMatrix();
		_glsl.proj = params.camera.getProjMatrix();
        _glsl.viewPos = params.camera.getPosition();

        for (const auto object : params.world.getObjects(VertexSpecification::positionColor)) {
            _glsl.model = object->transform;
            _glsl.color = object->color;
            _glsl.userData = object->userData;
            uniformChecker.shouldCheck = true;
            object->model->draw();
        }
	}
}
