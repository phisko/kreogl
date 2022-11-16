#pragma once

// kreogl
#include "UIntSwapper.hpp"
#include "kreogl/impl/DefaultConstructors.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT Buffer : UIntSwapper {
		KREOGL_DEFAULT_MOVE(Buffer);

		Buffer() noexcept {
			KREOGL_PROFILING_SCOPE;
			glGenBuffers(1, &res);
		}

		~Buffer() noexcept {
			KREOGL_PROFILING_SCOPE;
			glDeleteBuffers(1, &res);
		}
	};
}