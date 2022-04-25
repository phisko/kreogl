#include "DirectionalLight.hpp"
#include "kreogl/Camera.hpp"

namespace kreogl {
    static glm::vec3 getCorrectDirection(const glm::vec3 & dir) noexcept {
        const auto normalized = glm::normalize(dir);
        return normalized.y == 1.f ?
               glm::vec3(.001f, .999f, 0.f) :
               normalized.y == -1.f ?
               glm::vec3(.001f, -.999f, 0.f) :
               dir;
    }

    glm::mat4 DirectionalLight::getLightSpaceMatrixForCascade(const DrawParams & params, size_t index) const noexcept {
        const auto resolution = params.camera.getViewport().getResolution();
        const float ar = (float)resolution.x / (float)resolution.y;
        const float tanHalfHFOV = std::tan(params.camera.getFOV() * ar / 2.f);
        const float tanHalfVFOV = std::tan(params.camera.getFOV() / 2.f);

        const float cascadeStart = index == 0 ? KREOGL_SHADOW_MAP_NEAR_PLANE : cascadeEnds[index - 1];
        const float cascadeEnd = cascadeEnds[index];

        const float xn = cascadeStart * tanHalfHFOV;
        const float yn = cascadeStart * tanHalfVFOV;

        const float xf = cascadeEnd * tanHalfHFOV;
        const float yf = cascadeEnd * tanHalfVFOV;

        const glm::vec3 frustumCorners[] = {
            // near face
            { xn, yn, -cascadeStart },
            { -xn, yn, -cascadeStart },
            { xn, -yn, -cascadeStart },
            { -xn, -yn, -cascadeStart },

            // far face
            { xf, yf, -cascadeEnd },
            { -xf, yf, -cascadeEnd },
            { xf, -yf, -cascadeEnd },
            { -xf, -yf, -cascadeEnd }
        };

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::min();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::min();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::min();

        const auto dir = getCorrectDirection(direction);

        const auto lightView = glm::lookAt({ 0.f, 0.f, 0.f }, dir, { 0.f, 1.f, 0.f });
        const auto inverseView = glm::inverse(params.camera.getViewMatrix());

        for (const auto & corner : frustumCorners) {
            const auto worldPos = inverseView * glm::vec4(corner, 1.f);
            const auto lightPos = lightView * worldPos;

            minX = std::min(minX, lightPos.x);
            maxX = std::max(maxX, lightPos.x);
            minY = std::min(minY, lightPos.y);
            maxY = std::max(maxY, lightPos.y);
            minZ = std::min(minZ, lightPos.z);
            maxZ = std::max(maxZ, lightPos.z);
        }

        const auto lightProj = glm::ortho(minX, maxX, minY, maxY, minZ - shadowCasterMaxDistance, maxZ + shadowCasterMaxDistance);
        return lightProj * lightView;
    }
}