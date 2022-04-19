#include "kreogl.hpp"

// glfw
#include <GLFW/glfw3.h>

// stl
#include <iostream>

namespace kreogl {
    bool init() noexcept {
        glfwSetErrorCallback([](int error, const char * desc) {
            std::cerr << "GLFW error -- Code: " << error << ". Description: '" << desc << '\'' << std::endl;
        });

        if (glfwInit() != GLFW_TRUE) {
            std::cerr << "kreogl: failed to init GLFW" << std::endl;
            return false;
        }

        return true;
    }

    void pollEvents() noexcept {
        glfwPollEvents();
    }
}