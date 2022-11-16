#pragma once

#include <vector>
#include <GL/glew.h>

namespace kreogl {
	struct VertexSpecification {
		struct Attribute {
			GLenum type;
			size_t elementCount;
		};

		std::vector<Attribute> attributes;

		static KREOGL_EXPORT const VertexSpecification positionColor;
		static KREOGL_EXPORT const VertexSpecification skeletal;
	};
}
