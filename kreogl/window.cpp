#include "window.hpp"

// stl
#include <algorithm>

// gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// kreogl
#include "kreogl/impl/kreogl_init.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	window::window(const construction_params & params) noexcept
	: _size(params.size)
	{
		KREOGL_PROFILING_SCOPE;

		init_glfw();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#ifndef NDEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

		glfwWindowHint(GLFW_RESIZABLE, params.resizable ? GLFW_TRUE : GLFW_FALSE);

		// TODO: add fullscreen param

		init(glfwCreateWindow(params.size.x, params.size.y, params.name, nullptr, nullptr));
		_glfw_window.owning = true;
	}

	window::window(GLFWwindow & glfw_window) noexcept {
		KREOGL_PROFILING_SCOPE;

		init(&glfw_window);
	}

	void window::init(GLFWwindow * glfw_window) noexcept {
		KREOGL_PROFILING_SCOPE;

		_glfw_window.window = glfw_window;
		glfwGetWindowSize(_glfw_window, &_size.x, &_size.y);
		glfwMakeContextCurrent(_glfw_window);
		init_glew();
		_default_camera = std::make_unique<camera>(camera::construction_params{
			.viewport = {
				.resolution = _size
			}
		});
		_cameras.emplace_back(_default_camera.get());
	}

	void window::draw(const world & world, const shader_pipeline & shader_pipeline) noexcept {
		KREOGL_PROFILING_SCOPE;

		prepare_for_draw();
		for (const auto camera : _cameras)
			draw_world_to_camera(world, *camera, shader_pipeline);
	}

	void window::prepare_for_draw() noexcept {
		KREOGL_PROFILING_SCOPE;

		glfwMakeContextCurrent(_glfw_window);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::ranges::sort(_cameras, [](const camera * lhs, const camera * rhs) noexcept {
			return lhs->get_viewport().get_z_order() < rhs->get_viewport().get_z_order();
		});
	}

	void window::draw_world_to_camera(const world & world, const camera & camera, const shader_pipeline & shader_pipeline) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto & viewport = camera.get_viewport();
		viewport.draw({ world, camera, shader_pipeline });
		blit_viewport(viewport);
	}

	void window::display() const noexcept {
		KREOGL_PROFILING_SCOPE;

		glfwSwapBuffers(_glfw_window);
	}

	void window::blit_viewport(const viewport & viewport) const noexcept {
		KREOGL_PROFILING_SCOPE;

		glBindFramebuffer(GL_READ_FRAMEBUFFER, viewport.get_frame_buffer());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		const auto dest_size_x = (GLint)(viewport.get_on_screen_size().x * (float)_size.x);
		const auto dest_size_y = (GLint)(viewport.get_on_screen_size().y * (float)_size.y);

		const auto dest_x = (GLint)(viewport.get_on_screen_position().x * (float)_size.x);
		// OpenGL coords have Y=0 at the bottom, I want Y=0 at the top
		const auto dest_y = (GLint)(_size.y - dest_size_y - (GLint)(viewport.get_on_screen_position().y * (float)_size.y));

		glBlitFramebuffer(
			// src
			0, 0, viewport.get_resolution().x, viewport.get_resolution().y,
			// dest
			dest_x, dest_y, dest_x + dest_size_x, dest_y + dest_size_y,
			GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	bool window::should_close() const noexcept {
		KREOGL_PROFILING_SCOPE;
		return glfwWindowShouldClose(_glfw_window);
	}

	void window::poll_events() noexcept {
		KREOGL_PROFILING_SCOPE;
		glfwPollEvents();
	}

	void window::add_camera(const camera & camera) noexcept {
		KREOGL_PROFILING_SCOPE;
		_cameras.push_back(&camera);
	}

	void window::remove_camera(const camera & camera) noexcept {
		KREOGL_PROFILING_SCOPE;
		const auto it = std::ranges::find(_cameras, &camera);
		if (it != _cameras.end())
			_cameras.erase(it);
	}
}