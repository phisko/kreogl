#pragma once

// stl
#include <vector>

// gl
#include <GL/glew.h>

namespace kreogl {
	struct vertex_specification {
		struct attribute {
			GLenum type;
			size_t element_count;
		};

		std::vector<attribute> attributes;

		static KREOGL_EXPORT const vertex_specification position_color;
		static KREOGL_EXPORT const vertex_specification skeletal;
	};
}
