#pragma once

// kreogl
#include "kreogl/impl/raii/frame_buffer.hpp"
#include "kreogl/impl/raii/texture.hpp"

#ifndef KREOGL_MAX_CSM_COUNT
#	define KREOGL_MAX_CSM_COUNT 5
#endif

namespace kreogl {
	struct KREOGL_EXPORT cascaded_shadow_map {
		cascaded_shadow_map() noexcept;

		kreogl::frame_buffer frame_buffer;
		texture textures[KREOGL_MAX_CSM_COUNT];

		void set_size(int size) noexcept;
		int get_size() const noexcept { return _size; }

	private:
		int _size = -1;
	};
}