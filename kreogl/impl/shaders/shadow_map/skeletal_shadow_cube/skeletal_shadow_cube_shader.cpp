#include "skeletal_shadow_cube_shader.hpp"

// kreogl
#include "kreogl/impl/shaders/vertex_specification.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	skeletal_shadow_cube_shader::skeletal_shadow_cube_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("skeletal_shadow_cube_shader");
		use_without_uniform_check();
		skeletal_shader::_glsl.proj = glm::mat4(1.f);
		skeletal_shader::_glsl.view = glm::mat4(1.f);
	}

	void skeletal_shadow_cube_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		shadow_cube_shader::add_source_files();
		add_source_file(skeletal_glsl::vert, GL_VERTEX_SHADER);
	}

	std::vector<uniform_base *> skeletal_shadow_cube_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		auto uniforms = shadow_cube_shader::get_uniforms();
		uniforms.push_back(&skeletal_shader::_glsl.proj);
		uniforms.push_back(&skeletal_shader::_glsl.view);
		uniforms.push_back(&skeletal_shader::_glsl.model);
		uniforms.push_back(&skeletal_shader::_glsl.bones);
		return uniforms;
	}

	void skeletal_shadow_cube_shader::draw_objects(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		for (const auto object : params.world.get_objects(vertex_specification::skeletal))
			if (object->cast_shadows) {
				skeletal_shader::_glsl.model = object->transform;
				draw_object(*object);
			}
	}
}