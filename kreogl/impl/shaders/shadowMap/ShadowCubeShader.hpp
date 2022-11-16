#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/lights/DirectionalLight.hpp"
#include "kreogl/lights/SpotLight.hpp"
#include "kreogl/impl/DrawParams.hpp"
#include "kreogl/impl/shaders/shadowMap/ShadowCube/ShadowCubeGLSL.hpp"

namespace kreogl {
	class KREOGL_EXPORT ShadowCubeShader : public Shader {
	protected:
		virtual void drawObjects(const DrawParams & params) noexcept = 0;

	public:
		std::vector<UniformBase *> getUniforms() noexcept override;
		virtual void addSourceFiles() noexcept override;

	public:
		void draw(const PointLight & light, const DrawParams & params) noexcept;

	private:
		void draw(const DrawParams & params) noexcept final {}

	private:
		ShadowCubeGLSL _glsl;
	};
}