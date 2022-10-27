#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"
#include "DebugGLSL.hpp"

namespace kreogl {
	class DebugShader : public Shader, public Singleton<DebugShader> {
	public:
		DebugShader() noexcept;

	private:
		void draw(const DrawParams & params) noexcept override;
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;

	private:
		DebugGLSL _glsl;
	};
}