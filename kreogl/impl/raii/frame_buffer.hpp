#pragma once

// kreogl
#include "uint_swapper.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT frame_buffer : uint_swapper {
		frame_buffer() noexcept {
			KREOGL_PROFILING_SCOPE;
			glGenFramebuffers(1, &res);
		}

		~frame_buffer() noexcept {
			KREOGL_PROFILING_SCOPE;
			glDeleteFramebuffers(1, &res);
		}

		frame_buffer(frame_buffer &&) noexcept = default;
		frame_buffer & operator=(frame_buffer &&) noexcept = default;
	};
}