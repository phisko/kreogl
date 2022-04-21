#include "Window.hpp"

// stl
#include <algorithm>

// gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "impl/kreogl.hpp"

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::ranges::sort(_cameras, [](Camera * lhs, Camera * rhs) noexcept {
            return lhs->getViewport().getZOrder() < rhs->getViewport().getZOrder();
        });

        for (const auto camera : _cameras) {
            auto & viewport = camera->getViewport();
            viewport.draw({ world, *camera });

            glBindFramebuffer(GL_READ_FRAMEBUFFER, viewport.getFrameBuffer());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

            const auto destSizeX = (GLint)(viewport.getOnScreenSize().x * (float)_size.x);
            const auto destSizeY = (GLint)(viewport.getOnScreenSize().y * (float)_size.y);

            const auto destX = (GLint)(viewport.getOnScreenPosition().x * (float)_size.x);
            // OpenGL coords have Y=0 at the bottom, I want Y=0 at the top
            const auto destY = (GLint)((float)_size.y - destSizeY - viewport.getOnScreenPosition().y * (float)_size.y);

            glBlitFramebuffer(
                // src
                0, 0, viewport.getResolution().x, viewport.getResolution().y,
                // dest
                destX, destY, destX + destSizeX, destY + destSizeY,
                GL_COLOR_BUFFER_BIT, GL_LINEAR
            );
        }
    }

    bool Window::shouldClose() const noexcept {
        return glfwWindowShouldClose(_glfwWindow);
    }

    void Window::pollEvents() noexcept {
        glfwPollEvents();
    }
}