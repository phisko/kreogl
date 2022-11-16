#pragma once

// kreogl
#include "UIntSwapper.hpp"
#include "kreogl/impl/DefaultConstructors.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT VertexArray : UIntSwapper {
		KREOGL_DEFAULT_MOVE(VertexArray);

		VertexArray() noexcept {
			KREOGL_PROFILING_SCOPE;
			glGenVertexArrays(1, &res);
		}

		~VertexArray() noexcept {
			KREOGL_PROFILING_SCOPE;
			glDeleteVertexArrays(1, &res);
		}
	};
}