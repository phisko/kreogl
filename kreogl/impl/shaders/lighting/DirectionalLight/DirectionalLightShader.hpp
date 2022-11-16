#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

#include "DirectionalLightGLSL.hpp"
#include "kreogl/impl/shaders/shadowMap/sample/SampleCascadedShadowMap/SampleCascadedShadowMapGLSL.hpp"

namespace kreogl {
	class KREOGL_EXPORT DirectionalLightShader : public Shader, public Singleton<DirectionalLightShader> {
	public:
		DirectionalLightShader() noexcept;

	private:
		void draw(const DrawParams & params) noexcept override;
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;

	private:
		void updateShadowMap(const struct DirectionalLight & light, const DrawParams & params) noexcept;

	private:
		DirectionalLightGLSL _glsl;
		SampleCascadedShadowMapGLSL _csmGLSL;
	};
}