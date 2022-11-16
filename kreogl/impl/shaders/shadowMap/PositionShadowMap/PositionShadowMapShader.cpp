#include "PositionShadowMapShader.hpp"

// kreogl
#include "kreogl/Camera.hpp"
#include "kreogl/World.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	PositionShadowMapShader::PositionShadowMapShader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("PositionShadowMapShader");
		useWithoutUniformCheck();
		_glsl.proj = glm::mat4(1.f);
	}

	void PositionShadowMapShader::addSourceFiles() noexcept {
		KREOGL_PROFILING_SCOPE;

		addSourceFile(PositionProjViewModelGLSL::vert, GL_VERTEX_SHADER);
	}

	std::vector<UniformBase *> PositionShadowMapShader::getUniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			&_glsl.proj,
			&_glsl.view,
			&_glsl.model
		};
	}

	void PositionShadowMapShader::drawToTexture(GLuint texture, const glm::mat4 & lightSpaceMatrix, const DrawParams & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		_glsl.view = lightSpaceMatrix;

		for (const auto object : params.world.getObjects(VertexSpecification::positionColor))
			if (object->castShadows) {
				_glsl.model = object->transform;
				object->model->draw();
			}
	}
}
