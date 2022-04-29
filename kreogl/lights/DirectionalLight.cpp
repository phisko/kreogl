#include "DirectionalLight.hpp"
#include "kreogl/Camera.hpp"

#include <array>

namespace kreogl {
    static glm::vec3 getCorrectDirection(const glm::vec3 & dir) noexcept {
        const auto normalized = glm::normalize(dir);
        return normalized.y == 1.f ?
               glm::vec3(.001f, .999f, 0.f) :
               normalized.y == -1.f ?
               glm::vec3(.001f, -.999f, 0.f) :
               dir;
    }

    struct CascadeBounds {
        static constexpr auto cornerCount = 8;
        std::array<glm::vec4, cornerCount> corners;
        glm::vec3 center;
    };

    static CascadeBounds getCascadeBoundsWorldSpace(const glm::mat4 & proj, const glm::mat4 & view) noexcept {
        const auto inv = glm::inverse(proj * view);

        const glm::vec3 ndcCorners[] = {
            { -1.f, -1.f, -1.f }, // left bottom back
            { 1.f, -1.f, -1.f }, // right bottom back
            { -1.f, 1.f, -1.f, }, // left top back
            { 1.f, 1.f, -1.f }, // right top back
            { -1.f, -1.f, 1.f }, // left bottom front
            { 1.f, -1.f, 1.f }, // right bottom front
            { -1.f, 1.f, 1.f }, // left top front
            { 1.f, 1.f, 1.f }, // right top front
        };
        static_assert(std::extent_v<decltype(ndcCorners)> == CascadeBounds::cornerCount);

        CascadeBounds bounds;
        for (unsigned int i = 0; i < CascadeBounds::cornerCount; ++i) {
            const auto pt = inv * glm::vec4(ndcCorners[i], 1.f);
            bounds.corners[i] = pt / pt.w;
        }

        bounds.center = glm::vec3(0, 0, 0);
        for (const auto & v : bounds.corners)
            bounds.center += glm::vec3(v);
        bounds.center /= CascadeBounds::cornerCount;

        return bounds;
    }

    // based on https://learnopengl.com/Guest-Articles/2021/CSM
    glm::mat4 DirectionalLight::getLightSpaceMatrixForCascade(const DrawParams & params, size_t index) const noexcept {
        const auto nearPlane = index == 0 ? KREOGL_SHADOW_MAP_NEAR_PLANE : cascadeEnds[index - 1];
        const auto farPlane = cascadeEnds[index];

        const auto & resolution = params.camera.getViewport().getResolution();
        const auto proj = glm::perspective(params.camera.getFOV(), (float)resolution.x / (float)resolution.y, nearPlane, farPlane);

        const auto cascadeBoundsWorldSpace = getCascadeBoundsWorldSpace(proj, params.camera.getViewMatrix());

        const auto dir = getCorrectDirection(direction);
        const auto lightView = glm::lookAt(cascadeBoundsWorldSpace.center - dir, cascadeBoundsWorldSpace.center, { 0.f, 1.f, 0.f });

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::min();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::min();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::min();

        for (const auto & worldPos : cascadeBoundsWorldSpace.corners) {
            const auto lightPos = lightView * worldPos;

            minX = std::min(minX, lightPos.x);
            maxX = std::max(maxX, lightPos.x);
            minY = std::min(minY, lightPos.y);
            maxY = std::max(maxY, lightPos.y);
            minZ = std::min(minZ, lightPos.z);
            maxZ = std::max(maxZ, lightPos.z);
        }

        const auto lightProj = glm::ortho(
            minX - shadowCasterMaxDistance, maxX + shadowCasterMaxDistance,
            minY - shadowCasterMaxDistance, maxY + shadowCasterMaxDistance,
            minZ - shadowCasterMaxDistance, maxZ + shadowCasterMaxDistance
        );

        return lightProj * lightView;
    }
}