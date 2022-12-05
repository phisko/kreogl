#pragma once

// kreogl
#include "kreogl/animation/animated_model.hpp"
#include "kreogl/impl/shaders/helpers/skeletal/skeletal_shader.hpp"
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"

namespace kreogl {
	class KREOGL_EXPORT skeletal_textured_shader : public shader, public skeletal_shader, public singleton<skeletal_textured_shader> {
	public:
		skeletal_textured_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		void set_mesh_uniforms(const animated_model & model, unsigned int mesh_index) noexcept override;
		void bind_textures(const animated_model::mesh_textures & textures) noexcept;
	};
}
