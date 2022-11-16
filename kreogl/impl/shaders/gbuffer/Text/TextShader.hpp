#pragma once

#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/impl/shaders/Singleton.hpp"

#include "FontLibrary.hpp"
#include "TextGLSL.hpp"

#include "kreogl/Text.hpp"
#include "kreogl/impl/RAII/Buffer.hpp"
#include "kreogl/impl/RAII/VertexArray.hpp"

namespace kreogl {
	class KREOGL_EXPORT TextShader : public Shader, public Singleton<TextShader> {
	public:
		TextShader() noexcept;

	private:
		void draw(const DrawParams & params) noexcept override;
		void addSourceFiles() noexcept override;
		std::vector<UniformBase *> getUniforms() noexcept override;

	private:
		void draw(const DrawParams & params, const Text & text) noexcept;

	private:
		TextGLSL _glsl;
		Buffer _vbo;
		VertexArray _vao;
		FontLibrary _fonts;
	};
}
