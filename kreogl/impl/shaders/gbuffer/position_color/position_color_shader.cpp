#include "position_color_shader.hpp"

// kreogl
#include "kreogl/impl/shaders/helpers/apply_transparency/apply_transparency_glsl.hpp"
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	position_color_shader::position_color_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("position_color_shader");
	}

	void position_color_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(position_color_glsl::vert, GL_VERTEX_SHADER);
		add_source_file(position_color_glsl::frag, GL_FRAGMENT_SHADER);
		add_source_file(apply_transparency_glsl::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<uniform_base *> position_color_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			&_glsl.proj,
			&_glsl.view,
			&_glsl.model,
			&_glsl.view_pos,
			&_glsl.color,
			&_glsl.user_data
		};
	}

	void position_color_shader::draw(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto uniform_checker = use(false);

		_glsl.view = params.camera.get_view_matrix();
		_glsl.proj = params.camera.get_proj_matrix();
		_glsl.view_pos = params.camera.get_position();

		for (const auto object : params.world.get_objects(vertex_specification::position_color)) {
			_glsl.model = object->transform;
			_glsl.color = object->color;
			_glsl.user_data = object->user_data;
			uniform_checker.should_check = true;
			object->model->draw();
		}
	}
}
