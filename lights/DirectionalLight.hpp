#pragma once

#include "Light.hpp"
#include "impl/shadowMaps/CascadedShadowMap.hpp"
#include "impl/DrawParams.hpp"

namespace kreogl {
    struct DirectionalLight : Light {
        glm::vec3 direction{ 0, -1, 0 };
        float ambientStrength = .1f;
        std::vector<float> cascadeEnds = { 50 };
        float shadowCasterMaxDistance = 100.f;

        mutable CascadedShadowMap cascadedShadowMap;

        glm::mat4 getCascadedShadowMapLightSpaceMatrix(const DrawParams & params, size_t index) const noexcept;
    };
}