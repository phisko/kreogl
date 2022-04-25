#include "Window.hpp"

// stl
#include <algorithm>

// gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "kreogl/impl/kreogl.hpp"

namespace kreogl {
    Window::Window(const ConstructionParams& params) noexcept
        : _size(params.size)
    {
        initGLFW();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#ifndef _NDEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        glfwWindowHint(GLFW_RESIZABLE, params.resizable ? GLFW_TRUE : GLFW_FALSE);

        // TODO: add fullscreen param

        _glfwWindow = glfwCreateWindow(params.size.x, params.size.y, params.name, nullptr, nullptr);
        glfwSetWindowAspectRatio(_glfwWindow, params.size.x, params.size.y);

        glfwMakeContextCurrent(_glfwWindow);

        initGlew();

        if (params.defaultShaders)
            createDefaultShaders();

        _defaultCamera = std::make_unique<Camera>(Camera::ConstructionParams{
            .viewport = {
                .resolution = params.size
            }
        });
        _cameras.emplace_back(_defaultCamera.get());
    }

    Window::~Window() noexcept {
        glfwDestroyWindow(_glfwWindow);
    }

    void Window::draw(const World & world) noexcept {
        glfwMakeContextCurrent(_glfwWindow);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::ranges::sort(_cameras, [](const Camera * lhs, const Camera * rhs) noexcept {
            return lhs->getViewport().getZOrder() < rhs->getViewport().getZOrder();
        });

        for (const auto camera : _cameras) {
            const auto & viewport = camera->getViewport();
            viewport.draw({ world, *camera });
            blitViewport(viewport);
        }
    }

    void Window::display() const noexcept {
        glfwSwapBuffers(_glfwWindow);
    }

    void Window::blitViewport(const Viewport & viewport) const noexcept {
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
            GL_COLOR_BUFFER_BIT, GL_LINEAR
        );
    }

    bool Window::shouldClose() const noexcept {
        return glfwWindowShouldClose(_glfwWindow);
    }

    void Window::pollEvents() noexcept {
        glfwPollEvents();
    }

    void Window::addCamera(const Camera & camera) noexcept {
        _cameras.push_back(&camera);
    }

    void Window::removeCamera(const Camera &camera) noexcept {
        const auto it = std::ranges::find(_cameras, &camera);
        if (it != _cameras.end())
            _cameras.erase(it);
    }
}