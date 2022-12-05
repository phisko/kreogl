#include "debug_shader.hpp"

// kreogl
#include "kreogl/impl/shaders/helpers/apply_transparency/apply_transparency_glsl.hpp"
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/shaders/vertex_specification.hpp"
#include "kreogl/impl/shapes/line.hpp"
#include "kreogl/impl/shapes/sphere.hpp"
#include "kreogl/impl/shapes/box.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	debug_shader::debug_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("debug_shader");
	}

	void debug_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(debug_glsl::vert, GL_VERTEX_SHADER);
		add_source_file(debug_glsl::frag, GL_FRAGMENT_SHADER);
		add_source_file(apply_transparency_glsl::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<uniform_base *> debug_shader::get_uniforms() noexcept {
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

	void debug_shader::draw(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto uniform_checker = use(false);

		_glsl.view = params.camera.get_view_matrix();
		_glsl.proj = params.camera.get_proj_matrix();
		_glsl.view_pos = params.camera.get_position();

		for (const auto element : params.world.get_debug_elements()) {
			uniform_checker.should_check = true;

			_glsl.model = element->transform;
			_glsl.color = element->color;
			_glsl.user_data = element->user_data;

			switch (element->type) {
				case debug_element::type::line:
					kreogl::shapes::draw_line(element->line_start, element->line_end);
					break;
				case debug_element::type::sphere:
					kreogl::shapes::draw_sphere();
					break;
				case debug_element::type::box:
					kreogl::shapes::draw_box();
					break;
				default:
					assert(false); // Non-exhaustive switch
					break;
			}
		}
	}
}