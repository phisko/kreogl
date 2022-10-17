#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "Object.hpp"
#include "DebugElement.hpp"
#include "Text.hpp"
#include "Sprite.hpp"
#include "lights/DirectionalLight.hpp"
#include "lights/PointLight.hpp"
#include "lights/SpotLight.hpp"
#include "impl/texture/SkyboxTexture.hpp"

#define KREOGL_WORLD_COLLECTIONS(MACRO) \
    MACRO(DirectionalLight) \
    MACRO(PointLight) \
    MACRO(SpotLight) \
    MACRO(DebugElement) \
    MACRO(Text2D) \
    MACRO(Text3D) \
    MACRO(Sprite2D) \
    MACRO(Sprite3D)

namespace kreogl {
    class World {
    public:
        void add(const Object & object) noexcept;
        void remove(const Object & object) noexcept;
        const std::vector<const Object *> & getObjects(const VertexSpecification & vertexSpecification) const noexcept;

#define DECLARE_COLLECTION(T) \
        void add(const T & object) noexcept; \
        void remove(const T & object) noexcept; \
        const std::vector<const T *> & get##T##s() const noexcept;
        KREOGL_WORLD_COLLECTIONS(DECLARE_COLLECTION)
#undef DECLARE_COLLECTION

        struct Skybox {
            SkyboxTexture texture;
            glm::vec4 color{ 1.f };
        };
        Skybox skybox;

    private:
        std::unordered_map<const VertexSpecification *, std::vector<const Object *>> _objects;

#define COLLECTION_VECTOR(T) std::vector<const T *> _##T##s;
        KREOGL_WORLD_COLLECTIONS(COLLECTION_VECTOR)
#undef COLLECTION_VECTOR
    };
}