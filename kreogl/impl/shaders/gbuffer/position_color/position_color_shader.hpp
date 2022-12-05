#pragma once

// kreogl
#include "position_color_glsl.hpp"
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"

namespace kreogl {
	class KREOGL_EXPORT position_color_shader : public shader, public singleton<position_color_shader> {
	public:
		position_color_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		position_color_glsl _glsl;
	};
}
