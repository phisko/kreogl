#pragma once

// kreogl
#include "uint_swapper.hpp"
#include "kreogl/impl/default_constructors.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT vertex_array : uint_swapper {
		KREOGL_DEFAULT_MOVE(vertex_array);

		vertex_array() noexcept {
			KREOGL_PROFILING_SCOPE;
			glGenVertexArrays(1, &res);
		}

		~vertex_array() noexcept {
			KREOGL_PROFILING_SCOPE;
			glDeleteVertexArrays(1, &res);
		}
	};
}