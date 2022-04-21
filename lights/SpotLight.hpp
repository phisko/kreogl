#pragma once

#include "PointLight.hpp"
#include "impl/shadowMaps/ShadowMap.hpp"
#include "impl/DrawParams.hpp"

namespace kreogl {
    struct SpotLight : PointLight {
        glm::vec3 direction = { 0, -1, 0 };
        float cutOff = 1.f;
        float outerCutOff = 1.2f;

        mutable ShadowMap shadowMap;

        glm::mat4 getLightSpaceMatrix(const DrawParams & params) const noexcept;
    };
}