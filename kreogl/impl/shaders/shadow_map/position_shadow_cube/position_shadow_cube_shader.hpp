#pragma once

// kreogl
#include "kreogl/impl/shaders/singleton.hpp"
#include "kreogl/impl/shaders/shadow_map/shadow_cube_shader.hpp"
#include "kreogl/impl/shaders/helpers/position_proj_view_model/position_proj_view_model_glsl.hpp"

namespace kreogl {
	class KREOGL_EXPORT position_shadow_cube_shader : public shadow_cube_shader, public singleton<position_shadow_cube_shader> {
	public:
		position_shadow_cube_shader() noexcept;
		void draw_objects(const draw_params & params) noexcept override;

	private:
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		position_proj_view_model_glsl _glsl;
	};
}