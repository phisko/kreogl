#pragma once

// stl
#include <algorithm>
#include <unordered_map>
#include <vector>

// kreogl
#include "object.hpp"
#include "debug_element.hpp"
#include "text.hpp"
#include "sprite.hpp"
#include "lights/directional_light.hpp"
#include "lights/point_light.hpp"
#include "lights/spot_light.hpp"
#include "impl/texture/skybox_texture.hpp"

#define KREOGL_WORLD_COLLECTIONS(MACRO) \
	MACRO(directional_light) \
	MACRO(point_light) \
	MACRO(spot_light) \
	MACRO(debug_element) \
	MACRO(text_2d) \
	MACRO(text_3d) \
	MACRO(sprite_2d) \
	MACRO(sprite_3d)

namespace kreogl {
	class KREOGL_EXPORT world {
	public:
		void add(const object & object) noexcept;
		void remove(const object & object) noexcept;
		const std::vector<const object *> & get_objects(const vertex_specification & vertex_specification) const noexcept;

#define DECLARE_COLLECTION(T) \
	void add(const T & object) noexcept; \
	void remove(const T & object) noexcept; \
	const std::vector<const T *> & get_##T##s() const noexcept;
		KREOGL_WORLD_COLLECTIONS(DECLARE_COLLECTION)
#undef DECLARE_COLLECTION

		struct KREOGL_EXPORT skybox {
			const skybox_texture * texture = nullptr;
			glm::vec4 color{ 1.f };
		};
		skybox skybox;

	private:
		std::unordered_map<const vertex_specification *, std::vector<const object *>> _objects;

#define COLLECTION_VECTOR(T) std::vector<const T *> _##T##s;
		KREOGL_WORLD_COLLECTIONS(COLLECTION_VECTOR)
#undef COLLECTION_VECTOR
	};
}