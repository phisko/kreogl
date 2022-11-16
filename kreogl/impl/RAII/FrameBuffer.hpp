#pragma once

#include "UIntSwapper.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT FrameBuffer : UIntSwapper {
		FrameBuffer() noexcept {
			KREOGL_PROFILING_SCOPE;
			glGenFramebuffers(1, &res);
		}

		~FrameBuffer() noexcept {
			KREOGL_PROFILING_SCOPE;
			glDeleteFramebuffers(1, &res);
		}

		FrameBuffer(FrameBuffer &&) noexcept = default;
		FrameBuffer & operator=(FrameBuffer &&) noexcept = default;
	};
}