#pragma once

// kreogl
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"
#include "kreogl/impl/shaders/shadow_map/sample/sample_shadow_cube/sample_shadow_cube_glsl.hpp"
#include "kreogl/impl/shaders/lighting/point_light/point_light_glsl.hpp"
#include "kreogl/impl/shaders/post_lighting/volumetric_lighting/volumetric_lighting_glsl.hpp"

namespace kreogl {
	class KREOGL_EXPORT volumetric_point_light_shader : public shader, public singleton<volumetric_point_light_shader> {
	public:
		volumetric_point_light_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		volumetric_lighting_glsl _glsl;
		point_light_glsl::get_direction _point_light_glsl;
		sample_shadow_cube_glsl _shadow_cube_glsl;
	};
}