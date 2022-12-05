#pragma once

// kreogl
#include "kreogl/impl/raii/frame_buffer.hpp"
#include "kreogl/impl/raii/texture.hpp"

namespace kreogl {
	struct KREOGL_EXPORT shadow_map {
		shadow_map() noexcept;

		kreogl::frame_buffer frame_buffer;
		kreogl::texture texture;

		void set_size(int size) noexcept;
		int get_size() const noexcept { return _size; }

	private:
		int _size = -1;
	};
}