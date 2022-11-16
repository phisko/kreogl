#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

#include "SkyboxGLSL.hpp"

namespace kreogl {
	class KREOGL_EXPORT SkyboxShader : public Shader, public Singleton<SkyboxShader> {
	public:
		SkyboxShader() noexcept;

	private:
		void draw(const DrawParams & params) noexcept override;
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;

	private:
		void drawSkyBox() noexcept;

	private:
		SkyboxGLSL _glsl;
	};
}