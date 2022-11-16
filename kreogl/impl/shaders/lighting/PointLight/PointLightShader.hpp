#pragma once

#include "PointLightGLSL.hpp"
#include "kreogl/impl/shaders/helpers/PositionProjViewModel/PositionProjViewModelGLSL.hpp"
#include "kreogl/impl/shaders/shadowMap/sample/SampleShadowCube/SampleShadowCubeGLSL.hpp"

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

namespace kreogl {
	class KREOGL_EXPORT PointLightShader : public Shader, public Singleton<PointLightShader> {
	public:
		PointLightShader() noexcept;

	private:
		void draw(const DrawParams & params) noexcept override;
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;

	private:
		void updateShadowMap(const struct PointLight & light, const DrawParams & params) noexcept;

	private:
		PositionProjViewModelGLSL _ppvmGLSL;
		PointLightGLSL _glsl;
		SampleShadowCubeGLSL _shadowCubeGLSL;
	};
}
