#include "Camera.hpp"

namespace kreogl {
    Camera::Camera(const ConstructionParams & params) noexcept
        : _position(params.position)
        , _direction(params.direction)
        , _fov(params.fov)
        , _viewport(params.viewport)
        , _nearPlane(params.nearPlane)
        , _farPlane(params.farPlane)
    {}

    void Camera::setPosition(const glm::vec3 & position) noexcept {
        _position = position;
        _cachedViewMatrix = std::nullopt;
    }

    void Camera::setDirection(const glm::vec3 & direction) noexcept {
        _direction = direction;
        _cachedViewMatrix = std::nullopt;
    }

    void Camera::setFOV(float fov) noexcept {
        _fov = fov;
        _cachedProjMatrix = std::nullopt;
    }

    void Camera::setNearPlane(float nearPlane) noexcept {
        _nearPlane = nearPlane;
        _cachedProjMatrix = std::nullopt;
    }

    void Camera::setFarPlane(float farPlane) noexcept {
        _farPlane = farPlane;
        _cachedProjMatrix = std::nullopt;
    }

    Viewport & Camera::getViewportWritable() noexcept {
        _cachedProjMatrix = std::nullopt;
        return _viewport;
    }

    const glm::mat4 & Camera::getViewMatrix() const noexcept {
        if (_cachedViewMatrix)
            return *_cachedViewMatrix;
        _cachedViewMatrix = glm::lookAt(_position, _position + _direction, glm::vec3{ 0, 1, 0 });
        return *_cachedViewMatrix;
    }

    const glm::mat4 & Camera::getProjMatrix() const noexcept {
        if (_cachedProjMatrix)
            return *_cachedProjMatrix;
        _cachedProjMatrix = glm::perspective(_fov, _viewport.getAspectRatio(), _nearPlane, _farPlane);
        return *_cachedProjMatrix;
    }
}