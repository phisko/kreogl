#include "position_shadow_map_shader.hpp"

// kreogl
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	position_shadow_map_shader::position_shadow_map_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("position_shadow_map_shader");
		use_without_uniform_check();
		_glsl.proj = glm::mat4(1.f);
	}

	void position_shadow_map_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(position_proj_view_model_glsl::vert, GL_VERTEX_SHADER);
	}

	std::vector<uniform_base *> position_shadow_map_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			&_glsl.proj,
			&_glsl.view,
			&_glsl.model
		};
	}

	void position_shadow_map_shader::draw_to_texture(GLuint texture, const glm::mat4 & light_space_matrix, const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		_glsl.view = light_space_matrix;

		for (const auto object : params.world.get_objects(vertex_specification::position_color))
			if (object->cast_shadows) {
				_glsl.model = object->transform;
				object->model->draw();
			}
	}
}
