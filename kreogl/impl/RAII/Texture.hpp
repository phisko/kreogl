#pragma once

#include "UIntSwapper.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT Texture : UIntSwapper {
		Texture() noexcept {
			KREOGL_PROFILING_SCOPE;
			glGenTextures(1, &res);
		}

		~Texture() noexcept {
			KREOGL_PROFILING_SCOPE;
			glDeleteTextures(1, &res);
		}

		Texture(Texture &&) noexcept = default;
		Texture & operator=(Texture &&) noexcept = default;
	};
}