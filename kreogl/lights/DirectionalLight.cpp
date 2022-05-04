#include "DirectionalLight.hpp"
#include "kreogl/Camera.hpp"

#include <array>

#include "example/imgui/imgui.h"

#include "kreogl/Object.hpp"

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

    bool shouldSetCorners = true;
    Object * frustumCornerMarkers = nullptr;
    Object * lightOrthoCorners = nullptr;
    // based on https://learnopengl.com/Guest-Articles/2021/CSM
    glm::mat4 DirectionalLight::getLightSpaceMatrixForCascade(const DrawParams & params, size_t index) const noexcept {
        const auto nearPlane = index == 0 ? params.camera.getNearPlane() : std::max(params.camera.getNearPlane(), cascadeEnds[index - 1]);
        const auto farPlane = std::min(params.camera.getFarPlane(), cascadeEnds[index]);

        const auto & resolution = params.camera.getViewport().getResolution();
        const auto proj = glm::perspective(params.camera.getFOV(), (float)resolution.x / (float)resolution.y, nearPlane, farPlane);

        const auto cascadeBoundsWorldSpace = getCascadeBoundsWorldSpace(proj, params.camera.getViewMatrix());

        const auto dir = getCorrectDirection(direction);
        const auto lightView = glm::lookAt(cascadeBoundsWorldSpace.center - glm::normalize(dir), cascadeBoundsWorldSpace.center, { 0.f, 1.f, 0.f });

        glm::vec3 min(std::numeric_limits<float>::max());
        glm::vec3 max(std::numeric_limits<float>::min());

        size_t i = 0;
        for (const auto & worldPos : cascadeBoundsWorldSpace.corners) {
            const auto lightPos = lightView * worldPos;
            min = glm::min(min, glm::vec3(lightPos));
            max = glm::max(max, glm::vec3(lightPos));

            if (shouldSetCorners)
                frustumCornerMarkers[i++].transform = glm::translate(glm::mat4(1.f), glm::vec3(glm::inverse(lightView) * lightPos));
        }

        if (ImGui::Begin("Frustum corners")) {
            char label[12];
            sprintf_s(label, "%zd", index);
            if (ImGui::CollapsingHeader(label)) {
                ImGui::InputFloat3("Center", (float *)&cascadeBoundsWorldSpace.center);
                ImGui::Columns(2);
                for (auto worldPos : cascadeBoundsWorldSpace.corners) {
                    ImGui::InputFloat3("World", &worldPos.x);
                    ImGui::NextColumn();
                    auto lightPos = lightView * worldPos;
                    ImGui::InputFloat3("Light", &lightPos.x);
                    ImGui::NextColumn();
                }
                ImGui::Columns();
            }
        }
        ImGui::End();

        if (shouldSetCorners) {
            lightOrthoCorners[0].transform = glm::translate(glm::mat4(1.f), glm::vec3(glm::inverse(lightView) * glm::vec4(min.x, min.y, min.z, 1.f)));
            lightOrthoCorners[1].transform = glm::translate(glm::mat4(1.f), glm::vec3(glm::inverse(lightView) * glm::vec4(max.x, min.y, min.z, 1.f)));
            lightOrthoCorners[2].transform = glm::translate(glm::mat4(1.f), glm::vec3(glm::inverse(lightView) * glm::vec4(min.x, max.y, min.z, 1.f)));
            lightOrthoCorners[3].transform = glm::translate(glm::mat4(1.f), glm::vec3(glm::inverse(lightView) * glm::vec4(min.x, min.y, max.z, 1.f)));
            lightOrthoCorners[4].transform = glm::translate(glm::mat4(1.f), glm::vec3(glm::inverse(lightView) * glm::vec4(max.x, max.y, min.z, 1.f)));
            lightOrthoCorners[5].transform = glm::translate(glm::mat4(1.f), glm::vec3(glm::inverse(lightView) * glm::vec4(max.x, min.y, max.z, 1.f)));
            lightOrthoCorners[6].transform = glm::translate(glm::mat4(1.f), glm::vec3(glm::inverse(lightView) * glm::vec4(max.x, max.y, max.z, 1.f)));
            lightOrthoCorners[7].transform = glm::translate(glm::mat4(1.f), glm::vec3(glm::inverse(lightView) * glm::vec4(min.x, max.y, max.z, 1.f)));
        }
        shouldSetCorners = false;

        const auto largestExtent = glm::max(glm::abs(min), glm::abs(max));
        const auto lightProj = glm::ortho(-largestExtent.x, largestExtent.x, -largestExtent.y, largestExtent.y, -largestExtent.z - shadowCasterMaxDistance, largestExtent.z + shadowCasterMaxDistance);

        return lightProj * lightView;
    }
}