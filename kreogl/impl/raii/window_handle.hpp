#pragma once

// glfw
#include <GLFW/glfw3.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT window_handle {
		GLFWwindow * window = nullptr;
		bool owning = false;

		window_handle() noexcept = default;

		~window_handle() noexcept {
			KREOGL_PROFILING_SCOPE;

			if (owning)
				glfwDestroyWindow(window);
		}

		window_handle(window_handle && rhs) noexcept {
			KREOGL_PROFILING_SCOPE;

			std::swap(window, rhs.window);
			std::swap(owning, rhs.owning);
		}

		window_handle & operator=(window_handle && rhs) noexcept {
			KREOGL_PROFILING_SCOPE;

			std::swap(window, rhs.window);
			std::swap(owning, rhs.owning);
			return *this;
		}

		operator GLFWwindow *() const noexcept { return window; }

		explicit window_handle(GLFWwindow * window) noexcept
			: window(window), owning(false) {}
	};
}