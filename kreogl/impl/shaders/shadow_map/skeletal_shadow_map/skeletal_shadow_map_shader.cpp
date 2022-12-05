#include "skeletal_shadow_map_shader.hpp"

// kreogl
#include "kreogl/world.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	skeletal_shadow_map_shader::skeletal_shadow_map_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("skeletal_shadow_map_shader");
		use_without_uniform_check();
		_glsl.proj = glm::mat4(1.f);
	}

	void skeletal_shadow_map_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(skeletal_glsl::vert, GL_VERTEX_SHADER);
	}

	std::vector<uniform_base *> skeletal_shadow_map_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			&_glsl.model,
			&_glsl.view,
			&_glsl.proj,
			&_glsl.bones
		};
	}

	void skeletal_shadow_map_shader::draw_to_texture(GLuint texture, const glm::mat4 & light_space_matrix, const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		_glsl.view = light_space_matrix;

		for (const auto object : params.world.get_objects(vertex_specification::skeletal))
			if (object->cast_shadows) {
				_glsl.model = object->transform;
				draw_object(*object);
			}
	}
}