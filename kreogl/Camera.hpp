#pragma once

#include <optional>
#include <glm/gtc/matrix_transform.hpp>
#include "kreogl/impl/Viewport.hpp"

namespace kreogl {
    class Camera {
    public:
        struct ConstructionParams {
            glm::vec3 position = { 0, 0, 0 };
            glm::vec3 direction = { 0, 0, 1 };
            float fov = 1.f;
            Viewport::ConstructionParams viewport = {};
            float nearPlane = 1.f;
            float farPlane = 1000.f;
        };
        Camera(const ConstructionParams & params) noexcept;

    public:
        const glm::vec3 & getPosition() const noexcept { return _position; }
        void setPosition(const glm::vec3 & position) noexcept;

        const glm::vec3 & getDirection() const noexcept { return _direction; }
        void setDirection(const glm::vec3 & direction) noexcept;

        float getFOV() const noexcept { return _fov; }
        void setFOV(float fov) noexcept;

        const Viewport & getViewport() const noexcept { return _viewport; }
        Viewport & getViewportWritable() noexcept;

        float getNearPlane() const noexcept { return _nearPlane; }
        void setNearPlane(float nearPlane) noexcept;

        float getFarPlane() const noexcept { return _farPlane; }
        void setFarPlane(float farPlane) noexcept;

        const glm::mat4 & getViewMatrix() const noexcept;
        const glm::mat4 & getProjMatrix() const noexcept;

    private:
        glm::vec3 _position;
        glm::vec3 _direction;
        float _fov;
        Viewport _viewport;

        float _nearPlane;
        float _farPlane;

        mutable std::optional<glm::mat4> _cachedViewMatrix;
        mutable std::optional<glm::mat4> _cachedProjMatrix;
    };
}