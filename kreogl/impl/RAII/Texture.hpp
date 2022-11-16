#pragma once

// kreogl
#include "UIntSwapper.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"
#include "kreogl/impl/DefaultConstructors.hpp"

namespace kreogl {
	struct KREOGL_EXPORT Texture : UIntSwapper {
		KREOGL_DEFAULT_MOVE(Texture);

		Texture() noexcept {
			KREOGL_PROFILING_SCOPE;
			glGenTextures(1, &res);
		}

		~Texture() noexcept {
			KREOGL_PROFILING_SCOPE;
			glDeleteTextures(1, &res);
		}
	};
}