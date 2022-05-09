#pragma once

#include "PointLight.hpp"
#include "kreogl/impl/shadowMaps/ShadowMap.hpp"
#include "kreogl/impl/DrawParams.hpp"

namespace kreogl {
    struct SpotLight : PointLight {
        glm::vec3 direction;
        float cutOff = .91f;
        float outerCutOff = .82f;

        mutable ShadowMap shadowMap;

        glm::mat4 getLightSpaceMatrix(const DrawParams & params) const noexcept;
    };
}