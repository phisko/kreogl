#include <algorithm>
#include "World.hpp"

namespace kreogl {
    void World::add(const Object & object) noexcept {
        _objects[object.model->vertexSpecification].push_back(&object);
    }

    void World::remove(const Object & object) noexcept {
        auto & objects = _objects[object.model->vertexSpecification];
        const auto it = std::ranges::find(objects, &object);
        if (it != objects.end())
            objects.erase(it);
    }

    const std::vector<const Object *> & World::getObjects(VertexSpecification vertexSpecification) const noexcept {
        static const std::vector<const Object *> empty;
        const auto it = _objects.find(vertexSpecification);
        if (it == _objects.end())
            return empty;
        return it->second;
    }

    void World::add(const DirectionalLight & light) noexcept {
        _directionalLights.push_back(&light);
    }

    void World::remove(const DirectionalLight & light) noexcept {
        const auto it = std::ranges::find(_directionalLights, &light);
        if (it != _directionalLights.end())
            _directionalLights.erase(it);
    }

    const std::vector<const DirectionalLight *> & World::getDirectionalLights() const noexcept {
        return _directionalLights;
    }

    void World::add(const PointLight & light) noexcept {
        _pointLights.push_back(&light);
    }

    void World::remove(const PointLight & light) noexcept {
        const auto it = std::ranges::find(_pointLights, &light);
        if (it != _pointLights.end())
            _pointLights.erase(it);
    }

    const std::vector<const PointLight *> & World::getPointLights() const noexcept {
        return _pointLights;
    }

    void World::add(const SpotLight & light) noexcept {
        _spotLights.push_back(&light);
    }

    void World::remove(const SpotLight & light) noexcept {
        const auto it = std::ranges::find(_spotLights, &light);
        if (it != _spotLights.end())
            _spotLights.erase(it);
    }

    const std::vector<const SpotLight *> & World::getSpotLights() const noexcept {
        return _spotLights;
    }
}