#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

#include "kreogl/impl/shaders/helpers/PositionProjViewModel/PositionProjViewModelGLSL.hpp"
#include "kreogl/impl/shaders/helpers/Color/ColorGLSL.hpp"

namespace kreogl {
	class LightSphereShader : public Shader, public Singleton<LightSphereShader> {
	public:
		LightSphereShader() noexcept;

	private:
		void draw(const DrawParams & params) noexcept override;
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;

	private:
		void drawLight(const struct Light & light, const glm::vec3 & pos) noexcept;

	private:
		PositionProjViewModelGLSL _glsl;
		ColorGLSL _colorGLSL;
	};
}