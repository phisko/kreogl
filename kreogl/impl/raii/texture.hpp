#pragma once

// kreogl
#include "uint_swapper.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"
#include "kreogl/impl/default_constructors.hpp"

namespace kreogl {
	struct KREOGL_EXPORT texture : uint_swapper {
		KREOGL_DEFAULT_MOVE(texture);

		texture() noexcept {
			KREOGL_PROFILING_SCOPE;
			glGenTextures(1, &res);
		}

		~texture() noexcept {
			KREOGL_PROFILING_SCOPE;
			glDeleteTextures(1, &res);
		}
	};
}