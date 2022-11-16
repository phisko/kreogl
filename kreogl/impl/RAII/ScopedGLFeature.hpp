#pragma once

// gl
#include <GL/glew.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	struct KREOGL_EXPORT ScopedGLFeature {
		ScopedGLFeature(GLenum feature) noexcept
		: feature(feature)
		{
			KREOGL_PROFILING_SCOPE;
			glEnable(feature);
		}

		~ScopedGLFeature() {
			KREOGL_PROFILING_SCOPE;
			glDisable(feature);
		}

		GLenum feature;
	};
}