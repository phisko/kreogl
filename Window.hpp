#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Camera.hpp"

struct GLFWwindow;

namespace kreogl {
    class Window {
    public:
        struct ConstructionParams {
            const char * name = "kreogl";
            glm::ivec2 size = { 1280, 720 };
            bool resizable = false;
            bool defaultShaders = true;
        };

        Window(const ConstructionParams& params) noexcept;
        ~Window() noexcept;

        void draw(const class World & world) noexcept;
        bool shouldClose() const noexcept;

        // polls events for all windows
        static void pollEvents() noexcept;

        Camera & getDefaultCamera() noexcept { return *_defaultCamera; }
        void addCamera(const Camera & camera) noexcept;

        GLFWwindow * getGLFWwindow() const noexcept { return _glfwWindow; }

    private:
        glm::ivec2 _size;
        GLFWwindow * _glfwWindow;
        mutable std::vector<const Camera *> _cameras; // gets sorted in `draw`
        std::unique_ptr<Camera> _defaultCamera;
    };
}