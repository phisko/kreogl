#include "Camera.hpp"

namespace kreogl {
    Camera::Camera(const ConstructionParams & params) noexcept
        : _viewport(params.viewport)
    {}

    const glm::mat4 & Camera::getViewMatrix() const noexcept {
        if (_cachedViewMatrix)
            return *_cachedViewMatrix;
        _cachedViewMatrix = glm::lookAt(_position, _position + _direction, glm::vec3{ 0, 1, 0 });
        return *_cachedViewMatrix;
    }

    const glm::mat4 & Camera::getProjMatrix() const noexcept {
        if (_cachedProjMatrix)
            return *_cachedProjMatrix;
        _cachedProjMatrix = glm::perspective(_fov, (float)_viewport.getResolution().x / (float)_viewport.getResolution().y, _nearPlane, _farPlane);
        return *_cachedProjMatrix;
    }
}