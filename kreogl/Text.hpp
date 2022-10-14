#pragma once

#include <string>
#include <glm/glm.hpp>

namespace kreogl {
    struct Text {
        std::string text;
        std::string font;
        size_t fontSize = 1024;

        enum class Alignment {
            Left,
            Center,
            Right
        };
        Alignment alignment = Alignment::Center;

        glm::mat4 transform{ 1.f };
        glm::vec4 color{ 1.f };
        glm::vec4 userData{ 0.f };
    };

    struct Text2D : Text {};
    struct Text3D : Text {};
}