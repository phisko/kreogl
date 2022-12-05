#pragma once

// kreogl
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"
#include "kreogl/impl/shaders/shadow_map/sample/sample_cascaded_shadow_map/sample_cascaded_shadow_map_glsl.hpp"
#include "kreogl/impl/shaders/lighting/directional_light/directional_light_glsl.hpp"
#include "kreogl/impl/shaders/post_lighting/volumetric_lighting/volumetric_lighting_glsl.hpp"

namespace kreogl {
	class KREOGL_EXPORT volumetric_directional_light_shader : public shader, public singleton<volumetric_directional_light_shader> {
	public:
		volumetric_directional_light_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		volumetric_lighting_glsl _glsl;
		directional_light_glsl::get_direction _dir_light_glsl;
		sample_cascaded_shadow_map_glsl _csm_glsl;
	};
}