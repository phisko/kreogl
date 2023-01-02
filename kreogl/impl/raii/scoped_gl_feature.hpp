#pragma once

// gl
#include <GL/glew.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT scoped_gl_feature {
		scoped_gl_feature(GLenum feature) noexcept
			: feature(feature) {
			KREOGL_PROFILING_SCOPE;
			glEnable(feature);
		}

		~scoped_gl_feature() {
			KREOGL_PROFILING_SCOPE;
			glDisable(feature);
		}

		GLenum feature;
	};
}