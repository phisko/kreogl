#pragma once

#include <glm/glm.hpp>
#include "texture/ImageTexture.hpp"

namespace kreogl {
    struct Sprite {
        const ImageTexture * texture = nullptr;
        glm::mat4 transform{ 1.f };
        glm::vec4 color{ 1.f };
        glm::vec4 userData{ 0.f };
    };

    struct Sprite2D : Sprite {};
    struct Sprite3D : Sprite {};
}