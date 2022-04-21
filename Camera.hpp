#pragma once

#include <optional>
#include <glm/gtc/matrix_transform.hpp>
#include "impl/Viewport.hpp"

namespace kreogl {
    class Camera {
    public:
        struct ConstructionParams {
            glm::vec3 position = { 0, 0, 0 };
            glm::vec3 direction = { 0, 0, 1 };
            float fov = 1.f;
            Viewport::ConstructionParams viewport = {};
        };
        Camera(const ConstructionParams & params) noexcept;

    public:
        const glm::vec3 & getPosition() const noexcept { return _position; }
        void setPosition(const glm::vec3 & position) noexcept { _position = position; }

        const glm::vec3 & getDirection() const noexcept { return _direction; }
        void setDirection(const glm::vec3 & direction) noexcept { _direction = direction; }

        float getFOV() const noexcept { return _fov; }
        void setFOV(float fov) noexcept { _fov = fov; }

        const Viewport & getViewport() const noexcept { return _viewport; }
        Viewport & getViewport() noexcept { return _viewport; }

        float getNearPlane() const noexcept { return _nearPlane; }
        void setNearPlane(float nearPlane) noexcept { _nearPlane = nearPlane; }

        float getFarPlane() const noexcept { return _farPlane; }
        void setFarPlane(float farPlane) noexcept { _farPlane = farPlane; }

        const glm::mat4 & getViewMatrix() const noexcept;
        const glm::mat4 & getProjMatrix() const noexcept;

    private:
        glm::vec3 _position;
        glm::vec3 _direction;
        float _fov;
        Viewport _viewport;

        float _nearPlane = 1.f;
        float _farPlane = 1000.f;

        mutable std::optional<glm::mat4> _cachedViewMatrix;
        mutable std::optional<glm::mat4> _cachedProjMatrix;
    };
}