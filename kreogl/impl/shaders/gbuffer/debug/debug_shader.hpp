#pragma once

// kreogl
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"
#include "debug_glsl.hpp"

namespace kreogl {
	class KREOGL_EXPORT debug_shader : public shader, public singleton<debug_shader> {
	public:
		debug_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		debug_glsl _glsl;
	};
}