#pragma once

// kreogl
#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"
#include "kreogl/impl/shaders/shadowMap/sample/SampleShadowMap/SampleShadowMapGLSL.hpp"
#include "kreogl/impl/shaders/lighting/PointLight/PointLightGLSL.hpp"
#include "kreogl/impl/shaders/postLighting/VolumetricLighting/VolumetricLightingGLSL.hpp"

namespace kreogl {
	class KREOGL_EXPORT VolumetricSpotLightShader : public Shader, public Singleton<VolumetricSpotLightShader> {
	public:
		VolumetricSpotLightShader() noexcept;

	private:
		void draw(const DrawParams & params) noexcept override;
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;

	private:
		VolumetricLightingGLSL _glsl;
		PointLightGLSL::GetDirection _pointLightGLSL;
		SampleShadowMapGLSL _shadowMapGLSL;
	};
}