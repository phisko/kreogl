#pragma once

// kreogl
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"
#include "directional_light_glsl.hpp"
#include "kreogl/impl/shaders/shadow_map/sample/sample_cascaded_shadow_map/sample_cascaded_shadow_map_glsl.hpp"

namespace kreogl {
	class KREOGL_EXPORT directional_light_shader : public shader, public singleton<directional_light_shader> {
	public:
		directional_light_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		void update_shadow_map(const struct directional_light & light, const draw_params & params) noexcept;

	private:
		directional_light_glsl _glsl;
		sample_cascaded_shadow_map_glsl _csm_glsl;
	};
}