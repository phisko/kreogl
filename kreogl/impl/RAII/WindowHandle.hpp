#pragma once

// glfw
#include <GLFW/glfw3.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT WindowHandle {
		GLFWwindow * window = nullptr;

		WindowHandle() noexcept = default;

		~WindowHandle() noexcept {
			KREOGL_PROFILING_SCOPE;

			glfwDestroyWindow(window);
		}

		WindowHandle(WindowHandle && rhs) noexcept {
			KREOGL_PROFILING_SCOPE;

			std::swap(window, rhs.window);
		}

		WindowHandle & operator=(WindowHandle && rhs) noexcept {
			KREOGL_PROFILING_SCOPE;

			std::swap(window, rhs.window);
			return *this;
		}

		operator GLFWwindow *() const noexcept { return window; }

		explicit WindowHandle(GLFWwindow * window) noexcept
		: window(window)
		{}
	};
}