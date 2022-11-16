#pragma once

// kreogl
#include "PositionColorGLSL.hpp"
#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

namespace kreogl {
	class KREOGL_EXPORT PositionColorShader : public Shader, public Singleton<PositionColorShader> {
	public:
		PositionColorShader() noexcept;

	private:
		void draw(const DrawParams & params) noexcept override;
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;

	private:
		PositionColorGLSL _glsl;
	};
}
