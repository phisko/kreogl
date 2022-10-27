#include "Window.hpp"

// stl
#include <algorithm>

// gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// kreogl
#include "kreogl/impl/kreogl_init.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	Window::Window(const ConstructionParams & params) noexcept
	: _size(params.size)
	{
		KREOGL_PROFILING_SCOPE;

		initGLFW();

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
	}

	Window::Window(GLFWwindow & glfwWindow) noexcept {
		KREOGL_PROFILING_SCOPE;

		init(&glfwWindow);
	}

	void Window::init(GLFWwindow * glfwWindow) noexcept {
		KREOGL_PROFILING_SCOPE;

		_glfwWindow.window = glfwWindow;
		glfwGetWindowSize(_glfwWindow, &_size.x, &_size.y);
		glfwMakeContextCurrent(_glfwWindow);
		initGlew();
		_defaultCamera = std::make_unique<Camera>(Camera::ConstructionParams{
			.viewport = {
				.resolution = _size
			}
		});
		_cameras.emplace_back(_defaultCamera.get());
	}

	void Window::draw(const World & world, const ShaderPipeline & shaderPipeline) noexcept {
		KREOGL_PROFILING_SCOPE;

		prepareForDraw();
		for (const auto camera : _cameras)
			drawWorldToCamera(world, *camera, shaderPipeline);
	}

	void Window::prepareForDraw() noexcept {
		KREOGL_PROFILING_SCOPE;

		glfwMakeContextCurrent(_glfwWindow);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::ranges::sort(_cameras, [](const Camera * lhs, const Camera * rhs) noexcept {
			return lhs->getViewport().getZOrder() < rhs->getViewport().getZOrder();
		});
	}

	void Window::drawWorldToCamera(const World & world, const Camera & camera, const ShaderPipeline & shaderPipeline) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto & viewport = camera.getViewport();
		viewport.draw({ world, camera, shaderPipeline });
		blitViewport(viewport);
	}

	void Window::display() const noexcept {
		KREOGL_PROFILING_SCOPE;

		glfwSwapBuffers(_glfwWindow);
	}

	void Window::blitViewport(const Viewport & viewport) const noexcept {
		KREOGL_PROFILING_SCOPE;

		glBindFramebuffer(GL_READ_FRAMEBUFFER, viewport.getFrameBuffer());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		const auto destSizeX = (GLint)(viewport.getOnScreenSize().x * (float)_size.x);
		const auto destSizeY = (GLint)(viewport.getOnScreenSize().y * (float)_size.y);

		const auto destX = (GLint)(viewport.getOnScreenPosition().x * (float)_size.x);
		// OpenGL coords have Y=0 at the bottom, I want Y=0 at the top
		const auto destY = (GLint)(_size.y - destSizeY - (GLint)(viewport.getOnScreenPosition().y * (float)_size.y));

		glBlitFramebuffer(
			// src
			0, 0, viewport.getResolution().x, viewport.getResolution().y,
			// dest
			destX, destY, destX + destSizeX, destY + destSizeY,
			GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	bool Window::shouldClose() const noexcept {
		KREOGL_PROFILING_SCOPE;
		return glfwWindowShouldClose(_glfwWindow);
	}

	void Window::pollEvents() noexcept {
		KREOGL_PROFILING_SCOPE;
		glfwPollEvents();
	}

	void Window::addCamera(const Camera & camera) noexcept {
		KREOGL_PROFILING_SCOPE;
		_cameras.push_back(&camera);
	}

	void Window::removeCamera(const Camera & camera) noexcept {
		KREOGL_PROFILING_SCOPE;
		const auto it = std::ranges::find(_cameras, &camera);
		if (it != _cameras.end())
			_cameras.erase(it);
	}
}