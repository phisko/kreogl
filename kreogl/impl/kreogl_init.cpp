#include "kreogl_init.hpp"

// gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// stl
#include <iostream>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	bool init_glfw() noexcept {
		KREOGL_PROFILING_SCOPE;

		static bool s_is_init = false;
		if (s_is_init)
			return true;

		glfwSetErrorCallback([](int error, const char * desc) {
			std::cerr << "GLFW error -- Code: " << error << ". Description: '" << desc << '\'' << std::endl;
		});

		if (glfwInit() != GLFW_TRUE) {
			std::cerr << "kreogl: failed to init GLFW" << std::endl;
			return false;
		}

		s_is_init = true;
		return true;
	}

	bool init_glew() noexcept {
		KREOGL_PROFILING_SCOPE;

		static bool s_is_init = false;
		if (s_is_init)
			return true;

		glewExperimental = true;
		if (glewInit() != GLEW_OK) {
			std::cerr << "kreogl: failed to init glew" << std::endl;
			return false;
		}

#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(
			[](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * user_param) {
				if (id == 131154) // Pixel-path performance warning: Pixel transfer is synchronized with 3D rendering
					return;

				const char * severity_string;
				if (severity == GL_DEBUG_SEVERITY_MEDIUM)
					severity_string = "warning";
				else if (severity == GL_DEBUG_SEVERITY_HIGH)
					severity_string = "error";
				else
					return;
				std::cerr << "OpenGL " << severity_string << " -- Source: " << source << ". Type: " << type << ". ID: " << id << ". Message: '" << message << '\'' << std::endl;
			},
			nullptr
		);
#endif

		s_is_init = true;
		return true;
	}
}