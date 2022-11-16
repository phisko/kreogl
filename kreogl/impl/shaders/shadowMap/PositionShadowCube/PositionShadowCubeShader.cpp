#include "PositionShadowCubeShader.hpp"

// kreogl
#include "kreogl/impl/shaders/VertexSpecification.hpp"
#include "kreogl/World.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	PositionShadowCubeShader::PositionShadowCubeShader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("PositionShadowCubeShader");
		useWithoutUniformCheck();
		_glsl.proj = glm::mat4(1.f);
		_glsl.view = glm::mat4(1.f);
	}

	void PositionShadowCubeShader::addSourceFiles() noexcept {
		KREOGL_PROFILING_SCOPE;

		ShadowCubeShader::addSourceFiles();
		addSourceFile(PositionProjViewModelGLSL::vert, GL_VERTEX_SHADER);
	}

	std::vector<UniformBase *> PositionShadowCubeShader::getUniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		auto uniforms = ShadowCubeShader::getUniforms();
		uniforms.push_back(&_glsl.proj);
		uniforms.push_back(&_glsl.view);
		uniforms.push_back(&_glsl.model);
		return uniforms;
	}

	void PositionShadowCubeShader::drawObjects(const DrawParams & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		for (const auto object : params.world.getObjects(VertexSpecification::positionColor))
			if (object->castShadows) {
				_glsl.model = object->transform;
				object->model->draw();
			}
	}
}