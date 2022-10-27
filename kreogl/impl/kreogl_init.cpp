#include "kreogl_init.hpp"

// gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// stl
#include <iostream>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	bool initGLFW() noexcept {
		KREOGL_PROFILING_SCOPE;

		static bool s_isInit = false;
		if (s_isInit)
			return true;

		glfwSetErrorCallback([](int error, const char * desc) {
			std::cerr << "GLFW error -- Code: " << error << ". Description: '" << desc << '\'' << std::endl;
		});

		if (glfwInit() != GLFW_TRUE) {
			std::cerr << "kreogl: failed to init GLFW" << std::endl;
			return false;
		}

		s_isInit = true;
		return true;
	}

	bool initGlew() noexcept {
		KREOGL_PROFILING_SCOPE;

		static bool s_isInit = false;
		if (s_isInit)
			return true;

		glewExperimental = true;
		if (glewInit() != GLEW_OK) {
			std::cerr << "kreogl: failed to init glew" << std::endl;
			return false;
		}

#ifndef NDEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam) {
			if (id == 131154) // Pixel-path performance warning: Pixel transfer is synchronized with 3D rendering
				return;

			const char * severityString;
			if (severity == GL_DEBUG_SEVERITY_MEDIUM)
				severityString = "warning";
			else if (severity == GL_DEBUG_SEVERITY_HIGH)
				severityString = "error";
			else
				return;
			std::cerr << "OpenGL " << severityString << " -- Source: " << source << ". Type: " << type << ". ID: " << id << ". Message: '" << message << '\'' << std::endl;
		}, nullptr);
#endif

		s_isInit = true;
		return true;
	}
}