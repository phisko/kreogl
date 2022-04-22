#pragma once

#include <unordered_map>
#include <vector>

#include "Object.hpp"
#include "lights/DirectionalLight.hpp"
#include "lights/PointLight.hpp"
#include "lights/SpotLight.hpp"
#include "impl/shaders/VertexSpecification.hpp"

namespace kreogl {
    class World {
    public:
        void add(const Object & object) noexcept;
        void remove(const Object & object) noexcept;
        const std::vector<const Object *> & getObjects(const VertexSpecification & vertexSpecification) const noexcept;

        void add(const DirectionalLight & light) noexcept;
        void remove(const DirectionalLight & light) noexcept;
        const std::vector<const DirectionalLight *> & getDirectionalLights() const noexcept;

        void add(const PointLight & light) noexcept;
        void remove(const PointLight & light) noexcept;
        const std::vector<const PointLight *> & getPointLights() const noexcept;

        void add(const SpotLight & light) noexcept;
        void remove(const SpotLight & light) noexcept;
        const std::vector<const SpotLight *> & getSpotLights() const noexcept;

    private:
        std::unordered_map<const VertexSpecification *, std::vector<const Object *>> _objects;
        std::vector<const DirectionalLight *> _directionalLights;
        std::vector<const PointLight *> _pointLights;
        std::vector<const SpotLight *> _spotLights;
    };
}