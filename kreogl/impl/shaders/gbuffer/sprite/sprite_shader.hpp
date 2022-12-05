#pragma once

// kreogl
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"
#include "sprite_glsl.hpp"
#include "kreogl/sprite.hpp"

namespace kreogl {
	class KREOGL_EXPORT sprite_shader : public shader, public singleton<sprite_shader> {
	public:
		sprite_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		void draw(const draw_params & params, const sprite & sprite) noexcept;

	private:
		sprite_glsl _glsl;
	};
}