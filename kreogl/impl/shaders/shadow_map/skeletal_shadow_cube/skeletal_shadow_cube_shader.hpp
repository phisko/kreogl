#pragma once

// kreogl
#include "kreogl/impl/shaders/helpers/skeletal/skeletal_shader.hpp"
#include "kreogl/impl/shaders/shadow_map/shadow_cube_shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"

namespace kreogl {
	class KREOGL_EXPORT skeletal_shadow_cube_shader : public shadow_cube_shader, public skeletal_shader, public singleton<skeletal_shadow_cube_shader> {
	public:
		skeletal_shadow_cube_shader() noexcept;

	private:
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;
		void draw_objects(const draw_params & params) noexcept override;
	};
}