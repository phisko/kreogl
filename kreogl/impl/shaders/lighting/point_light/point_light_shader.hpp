#pragma once

// kreogl
#include "point_light_glsl.hpp"
#include "kreogl/impl/shaders/helpers/position_proj_view_model/position_proj_view_model_glsl.hpp"
#include "kreogl/impl/shaders/shadow_map/sample/sample_shadow_cube/sample_shadow_cube_glsl.hpp"
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"

namespace kreogl {
	class KREOGL_EXPORT point_light_shader : public shader, public singleton<point_light_shader> {
	public:
		point_light_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		void update_shadow_map(const struct point_light & light, const draw_params & params) noexcept;

	private:
		position_proj_view_model_glsl _ppvm_glsl;
		point_light_glsl _glsl;
		sample_shadow_cube_glsl _shadow_cube_glsl;
	};
}
