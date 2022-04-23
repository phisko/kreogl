#pragma once

#include <glm/glm.hpp>
#include "kreogl/model/Model.hpp"

namespace kreogl {
    struct Object {
        const Model * model;
        glm::mat4 transform{ 1.f };
        glm::vec4 color { 1.f };
        glm::vec4 userData{ 0.f };
    };
}