#pragma once

// stl
#include <vector>
#include <memory>

// glm
#include <glm/glm.hpp>

// kreogl
#include "camera.hpp"
#include "kreogl/impl/shaders/shader_pipeline.hpp"
#include "kreogl/impl/raii/window_handle.hpp"

struct GLFWwindow;

namespace kreogl {
	class KREOGL_EXPORT window {
	public:
		struct construction_params {
			const char * name = "kreogl";
			glm::ivec2 size = { 1280, 720 };
			bool resizable = false;

			// Needed to avoid this error with gcc:
			// "Default member initializer for 'name' needed within definition of enclosing class 'window' outside of member functions"
			construction_params() noexcept {}
		};

		window(const construction_params & params = {}) noexcept;
		window(GLFWwindow & glfw_window) noexcept;

		// base API
	public:
		void draw(
			const class world & world,
			const shader_pipeline & shader_pipeline
#ifdef KREOGL_DEFAULT_SHADERS
			= shader_pipeline::get_default_shaders()
#endif
		) noexcept;
		void display() const noexcept;
		bool should_close() const noexcept;

		// polls events for all windows
		static void poll_events() noexcept;

		// detailed draw
	public:
		void prepare_for_draw() noexcept;
		void draw_world_to_camera(
			const class world & world,
			const camera & camera,
			const shader_pipeline & shader_pipeline
#ifdef KREOGL_DEFAULT_SHADERS
			= shader_pipeline::get_default_shaders()
#endif
		) noexcept;

		// cameras
	public:
		camera & get_default_camera() noexcept { return *_default_camera; }
		void add_camera(const camera & camera) noexcept;
		void remove_camera(const camera & camera) noexcept;
		const std::vector<const camera *> & get_cameras() const noexcept { return _cameras; }

	public:
		GLFWwindow * get_glfw_window() const noexcept { return _glfw_window.window; }

	private:
		void init(GLFWwindow * glfw_window) noexcept;
		void blit_viewport(const viewport & viewport) const noexcept;

	private:
		glm::ivec2 _size = { 0, 0 };
		window_handle _glfw_window;

	private:
		// cameras
		mutable std::vector<const camera *> _cameras; // gets sorted in `draw`
		std::unique_ptr<camera> _default_camera;
	};
}