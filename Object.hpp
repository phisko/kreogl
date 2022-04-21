#pragma once

#include <glm/glm.hpp>
#include "model/Model.hpp"

namespace kreogl {
    struct Object {
        const Model * model;
        glm::mat4 transform;
        glm::vec4 color;
        glm::vec4 userData;
    };
}