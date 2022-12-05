#pragma once

// stl
#include <utility>

// gl
#include <GL/glew.h>

namespace kreogl {
	struct KREOGL_EXPORT uint_swapper {
		GLuint res = (GLuint)-1;

		bool is_valid() const noexcept { return res != (GLuint)-1; }
		void set_invalid() noexcept { res = (GLuint)-1; }

		operator GLuint() const noexcept { return res; }

		GLuint & get() noexcept { return res; }
		const GLuint get() const noexcept { return res; }

		uint_swapper() noexcept = default;

		uint_swapper(uint_swapper && rhs) noexcept {
			std::swap(res, rhs.res);
		}

		uint_swapper & operator=(uint_swapper && rhs) noexcept {
			std::swap(res, rhs.res);
			return *this;
		}
	};
}