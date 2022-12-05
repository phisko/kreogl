#pragma once

// stl
#include <optional>

// kreogl
#include "kreogl/impl/raii/texture.hpp"

namespace kreogl {
	struct KREOGL_EXPORT image_texture {
		image_texture(const char * file) noexcept;
		image_texture(const void * data, unsigned int width, unsigned int height) noexcept;

		static bool is_supported_format(const char * file) noexcept;

		int width = 0;
		int height = 0;
		int components = 0;
		std::optional<kreogl::texture> texture;
	};
}