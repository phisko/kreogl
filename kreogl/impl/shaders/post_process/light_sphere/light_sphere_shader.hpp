#pragma once

// kreogl
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"
#include "kreogl/impl/shaders/helpers/position_proj_view_model/position_proj_view_model_glsl.hpp"
#include "kreogl/impl/shaders/helpers/color/color_glsl.hpp"

namespace kreogl {
	class KREOGL_EXPORT light_sphere_shader : public shader, public singleton<light_sphere_shader> {
	public:
		light_sphere_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		void draw_light(const struct light & light, const glm::vec3 & pos) noexcept;

	private:
		position_proj_view_model_glsl _glsl;
		color_glsl _color_glsl;
	};
}