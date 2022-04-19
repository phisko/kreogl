#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

namespace kreogl {
    class Window {
    public:
        struct ConstructionParams {
            const char * name = "kreogl";
            glm::ivec2 size = { 1280, 720 };
            bool resizable = false;
        };

        Window(const ConstructionParams& params) noexcept;
        ~Window() noexcept;

        bool shouldClose() const;

        GLFWwindow * getGLFWwindow() const {
            return _glfwWindow;
        }

    private:
        GLFWwindow * _glfwWindow;
    };
}