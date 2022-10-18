#include "Camera.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

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
        KREOGL_PROFILING_SCOPE;
        _position = position;
        _cachedViewMatrix = std::nullopt;
    }

    void Camera::setDirection(const glm::vec3 & direction) noexcept {
        KREOGL_PROFILING_SCOPE;
        _direction = direction;
        _cachedViewMatrix = std::nullopt;
    }

    void Camera::setFOV(float fov) noexcept {
        KREOGL_PROFILING_SCOPE;
        _fov = fov;
        _cachedProjMatrix = std::nullopt;
    }

    void Camera::setNearPlane(float nearPlane) noexcept {
        KREOGL_PROFILING_SCOPE;
        _nearPlane = nearPlane;
        _cachedProjMatrix = std::nullopt;
    }

    void Camera::setFarPlane(float farPlane) noexcept {
        KREOGL_PROFILING_SCOPE;
        _farPlane = farPlane;
        _cachedProjMatrix = std::nullopt;
    }

    Viewport & Camera::getViewportWritable() noexcept {
        KREOGL_PROFILING_SCOPE;
        _cachedProjMatrix = std::nullopt;
        return _viewport;
    }

    const glm::mat4 & Camera::getViewMatrix() const noexcept {
        KREOGL_PROFILING_SCOPE;
        if (_cachedViewMatrix)
            return *_cachedViewMatrix;
        _cachedViewMatrix = glm::lookAt(_position, _position + _direction, glm::vec3{ 0, 1, 0 });
        return *_cachedViewMatrix;
    }

    const glm::mat4 & Camera::getProjMatrix() const noexcept {
        KREOGL_PROFILING_SCOPE;
        if (_cachedProjMatrix)
            return *_cachedProjMatrix;
        _cachedProjMatrix = glm::perspective(_fov, _viewport.getAspectRatio(), _nearPlane, _farPlane);
        return *_cachedProjMatrix;
    }
}