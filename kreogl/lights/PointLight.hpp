#pragma once

#include "Light.hpp"
#include "kreogl/impl/shadowMaps/ShadowCube.hpp"

namespace kreogl {
    struct PointLight : Light {
        glm::vec3 position;
        float range = 1000.f;
        float constant = 1.f;
        float linear = .09f;
        float quadratic = .032f;

        float getRadius() const noexcept;

        mutable ShadowCube shadowCube;
    };
}