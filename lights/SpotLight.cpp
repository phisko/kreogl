#include "SpotLight.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"

namespace kreogl {
    static glm::vec3 getCorrectDirection(const glm::vec3 & dir) noexcept {
        const auto normalized = glm::normalize(dir);
        return normalized.y == 1.f ?
               glm::vec3(.001f, .999f, 0.f) :
               normalized.y == -1.f ?
               glm::vec3(.001f, -.999f, 0.f) :
               dir;
    }

    glm::mat4 SpotLight::getLightSpaceMatrix(const DrawParams & params) const noexcept {
        const auto & resolution = params.camera.getViewport().getResolution();
        const auto lightProjection = glm::perspective(45.f, (float)resolution.x / (float)resolution.y, KREOGL_SHADOW_MAP_NEAR_PLANE, KREOGL_SHADOW_MAP_FAR_PLANE);

        const auto dir = getCorrectDirection(direction);
        const auto lightView = glm::lookAt(position, position + dir, glm::vec3(0.f, 1.f, 0.f));

        return lightProjection * lightView;
    }
}