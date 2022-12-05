#pragma once

// kreogl
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"
#include "font_library.hpp"
#include "text_glsl.hpp"
#include "kreogl/text.hpp"
#include "kreogl/impl/raii/buffer.hpp"
#include "kreogl/impl/raii/vertex_array.hpp"

namespace kreogl {
	class KREOGL_EXPORT text_shader : public shader, public singleton<text_shader> {
	public:
		text_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		void draw(const draw_params & params, const text & text) noexcept;

	private:
		text_glsl _glsl;
		buffer _vbo;
		vertex_array _vao;
		font_library _fonts;
	};
}
