#include "Window.hpp"

// gl
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// stl
#include <iostream>

namespace kreogl {
    Window::Window(const ConstructionParams& params) noexcept {
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

        glewExperimental = true;
        if (glewInit() != GLEW_OK)
            std::cerr << "kreogl: failed to init glew" << std::endl;

#ifndef _NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const void * userParam) {
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
    }

    Window::~Window() noexcept {
        glfwDestroyWindow(_glfwWindow);
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(_glfwWindow);
    }
}