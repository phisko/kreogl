#pragma once

// kreogl
#include "kreogl/impl/raii/texture.hpp"

namespace kreogl {
	struct KREOGL_EXPORT texture_data {
		texture_data() noexcept = default;
		~texture_data() noexcept;

		texture_data(const char * file) noexcept;
		texture_data(const void * data, unsigned int width, unsigned int height) noexcept;

		texture_data(texture_data && other) noexcept;
		texture_data & operator=(texture_data && rhs) noexcept;

		texture load_to_texture() const noexcept;

		static bool is_supported_format(const char * file) noexcept;

		void * data = nullptr;
		int width = 0;
		int height = 0;
		int components = 0;
		bool must_release = true;
	};
}