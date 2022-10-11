#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "Camera.hpp"

#include "kreogl/impl/shaders/ShaderPipeline.hpp"

struct GLFWwindow;

namespace kreogl {
    class Window {
    public:
        struct ConstructionParams {
            const char * name = "kreogl";
            glm::ivec2 size = { 1280, 720 };
            bool resizable = false;

            // Needed to avoid this error with gcc:
            // "Default member initializer for 'name' needed within definition of enclosing class 'Window' outside of member functions"
            ConstructionParams() noexcept {}
        };

        Window(const ConstructionParams & params = ConstructionParams{}) noexcept;
        ~Window() noexcept;

        // base API
    public:
        void draw(
            const class World & world,
            const ShaderPipeline & shaderPipeline
#ifdef KREOGL_DEFAULT_SHADERS
                = ShaderPipeline::getDefaultShaders()
#endif
        ) noexcept;
        void display() const noexcept;
        bool shouldClose() const noexcept;

        // polls events for all windows
        static void pollEvents() noexcept;

        // cameras
    public:
        Camera & getDefaultCamera() noexcept { return *_defaultCamera; }
        void addCamera(const Camera & camera) noexcept;
        void removeCamera(const Camera & camera) noexcept;
        const std::vector<const Camera *> & getCameras() const noexcept { return _cameras; }

    public:
        GLFWwindow * getGLFWwindow() const noexcept { return _glfwWindow; }

    private:
        void blitViewport(const Viewport & viewport) const noexcept;

    private:
        glm::ivec2 _size;
        GLFWwindow * _glfwWindow;

    private:
        // cameras
        mutable std::vector<const Camera *> _cameras; // gets sorted in `draw`
        std::unique_ptr<Camera> _defaultCamera;
    };
}