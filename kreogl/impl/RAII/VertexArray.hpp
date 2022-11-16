#pragma once

// kreogl
#include "UIntSwapper.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT VertexArray : UIntSwapper {
		VertexArray() noexcept {
			KREOGL_PROFILING_SCOPE;
			glGenVertexArrays(1, &res);
		}

		~VertexArray() noexcept {
			KREOGL_PROFILING_SCOPE;
			glDeleteVertexArrays(1, &res);
		}

		VertexArray(VertexArray &&) noexcept = default;
		VertexArray & operator=(VertexArray &&) noexcept = default;
	};
}