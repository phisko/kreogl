#include "position_shadow_cube_shader.hpp"

// kreogl
#include "kreogl/impl/shaders/vertex_specification.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	position_shadow_cube_shader::position_shadow_cube_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("position_shadow_cube_shader");
		use_without_uniform_check();
		_glsl.proj = glm::mat4(1.f);
		_glsl.view = glm::mat4(1.f);
	}

	void position_shadow_cube_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		shadow_cube_shader::add_source_files();
		add_source_file(position_proj_view_model_glsl::vert, GL_VERTEX_SHADER);
	}

	std::vector<uniform_base *> position_shadow_cube_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		auto uniforms = shadow_cube_shader::get_uniforms();
		uniforms.push_back(&_glsl.proj);
		uniforms.push_back(&_glsl.view);
		uniforms.push_back(&_glsl.model);
		return uniforms;
	}

	void position_shadow_cube_shader::draw_objects(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		for (const auto object : params.world.get_objects(vertex_specification::position_color))
			if (object->cast_shadows) {
				_glsl.model = object->transform;
				object->model->draw();
			}
	}
}