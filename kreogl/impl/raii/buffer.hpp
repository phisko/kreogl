#pragma once

// kreogl
#include "uint_swapper.hpp"
#include "kreogl/impl/default_constructors.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT buffer : uint_swapper {
		KREOGL_DEFAULT_MOVE(buffer);

		buffer() noexcept {
			KREOGL_PROFILING_SCOPE;
			glGenBuffers(1, &res);
		}

		~buffer() noexcept {
			KREOGL_PROFILING_SCOPE;
			glDeleteBuffers(1, &res);
		}
	};
}