#include <algorithm>
#include "World.hpp"

namespace kreogl {
    void World::add(const Object & object) noexcept {
        auto & objects = _objects[&object.model->vertexSpecification];
        objects.push_back(&object);
    }

    void World::remove(const Object & object) noexcept {
        auto & objects = _objects[&object.model->vertexSpecification];
        const auto it = std::ranges::find(objects, &object);
        if (it != objects.end())
            objects.erase(it);
    }

    const std::vector<const Object *> & World::getObjects(const VertexSpecification & vertexSpecification) const noexcept {
        static const std::vector<const Object *> empty;
        const auto it = _objects.find(&vertexSpecification);
        if (it == _objects.end())
            return empty;
        return it->second;
    }

#define DEFINE_COLLECTION(T) \
    void World::add(const T & object) noexcept { \
        _##T##s.push_back(&object); \
    } \
    void World::remove(const T & object) noexcept { \
        const auto it = std::ranges::find(_##T##s, &object); \
        if (it != _##T##s.end()) \
            _##T##s.erase(it); \
    } \
    const std::vector<const T *> & World::get##T##s() const noexcept { \
        return _##T##s; \
    }

    KREOGL_WORLD_COLLECTIONS(DEFINE_COLLECTION)
#undef DEFINE_COLLECTION
}