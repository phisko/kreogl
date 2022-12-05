#pragma once

// kreogl
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/lights/directional_light.hpp"
#include "kreogl/lights/spot_light.hpp"
#include "kreogl/impl/draw_params.hpp"
#include "kreogl/impl/shaders/shadow_map/shadow_cube/shadow_cube_glsl.hpp"

namespace kreogl {
	class KREOGL_EXPORT shadow_cube_shader : public shader {
	protected:
		virtual void draw_objects(const draw_params & params) noexcept = 0;

	public:
		std::vector<uniform_base *> get_uniforms() noexcept override;
		virtual void add_source_files() noexcept override;

	public:
		void draw(const point_light & light, const draw_params & params) noexcept;

	private:
		void draw(const draw_params & params) noexcept final {}

	private:
		shadow_cube_glsl _glsl;
	};
}