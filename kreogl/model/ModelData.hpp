#pragma once

#include <functional>
#include <vector>
#include <GL/glew.h>

namespace kreogl {
	struct KREOGL_EXPORT MeshData {
		struct KREOGL_EXPORT Buffer {
			size_t nbElements;
			size_t elementSize;
			const void * data;
		};

		Buffer vertices;
		Buffer indices;
		GLenum indexType;
	};

	struct KREOGL_EXPORT ModelData {
		std::vector<MeshData> meshes;
		std::vector<size_t> vertexAttributeOffsets;
		size_t vertexSize;
	};
}