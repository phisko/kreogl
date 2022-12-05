#include "world.hpp"

// stl
#include <algorithm>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	void world::add(const object & object) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto & objects = _objects[&object.model->vertex_specification];
		objects.push_back(&object);
	}

	void world::remove(const object & object) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto & objects = _objects[&object.model->vertex_specification];
		const auto it = std::ranges::find(objects, &object);
		if (it != objects.end())
			objects.erase(it);
	}

	const std::vector<const object *> & world::get_objects(const vertex_specification & vertex_specification) const noexcept {
		KREOGL_PROFILING_SCOPE;

		static const std::vector<const object *> empty;
		const auto it = _objects.find(&vertex_specification);
		if (it == _objects.end())
			return empty;
		return it->second;
	}

#define DEFINE_COLLECTION(T) \
	void world::add(const T & object) noexcept { \
		KREOGL_PROFILING_SCOPE; \
		_##T##s.push_back(&object); \
	} \
	void world::remove(const T & object) noexcept { \
		KREOGL_PROFILING_SCOPE; \
		const auto it = std::ranges::find(_##T##s, &object); \
		if (it != _##T##s.end()) \
			_##T##s.erase(it); \
	} \
	const std::vector<const T *> & world::get_##T##s() const noexcept { \
		KREOGL_PROFILING_SCOPE; \
		return _##T##s; \
	}

	KREOGL_WORLD_COLLECTIONS(DEFINE_COLLECTION)
#undef DEFINE_COLLECTION
}