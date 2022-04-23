#pragma once

#include "Light.hpp"

namespace kreogl {
    struct PointLight : Light {
        glm::vec3 position;
        float range = 1000.f;
        float constant = 1.f;
        float linear = .09f;
        float quadratic = .032f;
    };
}