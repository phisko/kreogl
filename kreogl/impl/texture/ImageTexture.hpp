#pragma once

#include <optional>
#include "kreogl/impl/RAII/Texture.hpp"

namespace kreogl {
	struct KREOGL_EXPORT ImageTexture {
		ImageTexture(const char * file) noexcept;
		ImageTexture(const void * data, unsigned int width, unsigned int height) noexcept;

		static bool isSupportedFormat(const char * file) noexcept;

		int width = 0;
		int height = 0;
		int components = 0;
		std::optional<Texture> texture;
	};
}