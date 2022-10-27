#include "SkeletalShadowMapShader.hpp"

#include "kreogl/World.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	SkeletalShadowMapShader::SkeletalShadowMapShader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("SkeletalShadowMapShader");
		useWithoutUniformCheck();
		_glsl.proj = glm::mat4(1.f);
	}

	void SkeletalShadowMapShader::addSourceFiles() noexcept {
		KREOGL_PROFILING_SCOPE;

		addSourceFile(SkeletalGLSL::vert, GL_VERTEX_SHADER);
	}

	std::vector<UniformBase *> SkeletalShadowMapShader::getUniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			&_glsl.model,
			&_glsl.view,
			&_glsl.proj,
			&_glsl.bones
		};
	}

	void SkeletalShadowMapShader::drawToTexture(GLuint texture, const glm::mat4 & lightSpaceMatrix, const DrawParams & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		_glsl.view = lightSpaceMatrix;

		for (const auto object : params.world.getObjects(VertexSpecification::skeletal))
			if (object->castShadows) {
				_glsl.model = object->transform;
				drawObject(*object);
			}
	}
}