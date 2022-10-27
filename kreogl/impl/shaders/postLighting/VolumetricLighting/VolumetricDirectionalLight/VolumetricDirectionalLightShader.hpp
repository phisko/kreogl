#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

#include "kreogl/impl/shaders/shadowMap/sample/SampleCascadedShadowMap/SampleCascadedShadowMapGLSL.hpp"
#include "kreogl/impl/shaders/lighting/DirectionalLight/DirectionalLightGLSL.hpp"
#include "kreogl/impl/shaders/postLighting/VolumetricLighting/VolumetricLightingGLSL.hpp"

namespace kreogl {
	class VolumetricDirectionalLightShader : public Shader, public Singleton<VolumetricDirectionalLightShader> {
	public:
		VolumetricDirectionalLightShader() noexcept;

	private:
		void draw(const DrawParams & params) noexcept override;
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;

	private:
		VolumetricLightingGLSL _glsl;
		DirectionalLightGLSL::GetDirection _dirLightGLSL;
		SampleCascadedShadowMapGLSL _csmGLSL;
	};
}