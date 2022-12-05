#include "shader.hpp"

// stl
#include <iostream>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	void shader::init(const std::string & name) noexcept {
		KREOGL_PROFILING_SCOPE;

		_program = glCreateProgram();
#ifndef NDEBUG
		glObjectLabel(GL_PROGRAM, _program, (GLsizei)name.size(), name.c_str());
#endif
		add_source_files();
		link();
		_uniforms = get_uniforms();
		get_uniform_locations();
	}

	void shader::add_source_file(const char * glsl, GLenum type) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto shader = glCreateShader(type);
		glShaderSource(shader, 1, &glsl, nullptr);
		glCompileShader(shader);

#ifndef NDEBUG
		{
			char buffer[512];
			glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
			if (strlen(buffer) != 0) {
				std::cerr << "Error compiling program [" << typeid(this).name() << "]:" << std::endl
						  << '\t' << buffer << std::endl;
				assert(false);
			}

			GLint status;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
			assert(status == GL_TRUE);
		}
#endif

		glAttachShader(_program, shader);
	}

#ifndef NDEBUG
	shader::uniform_use_checker::~uniform_use_checker() noexcept {
		KREOGL_PROFILING_SCOPE;

		if (!should_check)
			return;

		for (const auto uniform : uniforms)
			if (!uniform->used)
				std::cerr << "kreogl: Unset uniform [" << uniform->name << ']' << std::endl;
	}
#endif

	shader::uniform_use_checker shader::use(bool should_check) noexcept {
		KREOGL_PROFILING_SCOPE;

		use_without_uniform_check();
		return {
#ifndef NDEBUG
			.uniforms = _uniforms,
			.should_check = should_check
#endif
		};
	}

	void shader::use_without_uniform_check() const noexcept {
		KREOGL_PROFILING_SCOPE;

		glUseProgram(_program);
	}

	void shader::link() const noexcept {
		KREOGL_PROFILING_SCOPE;

		glLinkProgram(_program);
#ifndef NDEBUG
		{
			char buffer[512];
			glGetProgramInfoLog(_program, sizeof(buffer), nullptr, buffer);
			if (strlen(buffer) != 0) {
				std::cerr << "Error linking shaders: '" << buffer << "'" << std::endl;
				assert(false);
			}

			GLint success;
			glGetProgramiv(_program, GL_LINK_STATUS, &success);
			assert(success == GL_TRUE);
		}
#endif
	}

	void shader::get_uniform_locations() noexcept {
		KREOGL_PROFILING_SCOPE;

		for (const auto uniform : _uniforms) {
			uniform->location = glGetUniformLocation(_program, uniform->name.c_str());
#ifndef NDEBUG
			if (uniform->location == -1)
				std::cerr << "Failed to get location for `" << uniform->name << "` uniform" << std::endl;
#endif
		}
	}
}