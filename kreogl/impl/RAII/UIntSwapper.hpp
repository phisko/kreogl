#pragma once

// stl
#include <utility>

// gl
#include <GL/glew.h>

namespace kreogl {
	struct KREOGL_EXPORT UIntSwapper {
		GLuint res = (GLuint)-1;

		bool isValid() const noexcept { return res != (GLuint)-1; }
		void setInvalid() noexcept { res = (GLuint)-1; }

		operator GLuint() const noexcept { return res; }

		GLuint & get() noexcept { return res; }
		const GLuint get() const noexcept { return res; }

		UIntSwapper() noexcept = default;

		UIntSwapper(UIntSwapper && rhs) noexcept {
			std::swap(res, rhs.res);
		}

		UIntSwapper & operator=(UIntSwapper && rhs) noexcept {
			std::swap(res, rhs.res);
			return *this;
		}
	};
}