#pragma once

// kreogl
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"
#include "kreogl/lights/spot_light.hpp"
#include "spot_light_glsl.hpp"
#include "kreogl/impl/shaders/helpers/position_proj_view_model/position_proj_view_model_glsl.hpp"
#include "kreogl/impl/shaders/shadow_map/sample/sample_shadow_map/sample_shadow_map_glsl.hpp"

namespace kreogl {
	class KREOGL_EXPORT spot_light_shader : public shader, public singleton<spot_light_shader> {
	public:
		spot_light_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		void update_shadow_map(const struct spot_light & light, const draw_params & params) noexcept;

	private:
		spot_light_glsl _glsl;
		position_proj_view_model_glsl _ppvm_glsl;
		sample_shadow_map_glsl _shadow_map_glsl;
	};
}