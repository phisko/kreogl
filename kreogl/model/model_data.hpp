#pragma once

// stl
#include <functional>
#include <vector>

// gl
#include <GL/glew.h>

namespace kreogl {
	struct KREOGL_EXPORT mesh_data {
		struct KREOGL_EXPORT buffer {
			size_t nb_elements;
			size_t element_size;
			const void * data;
		};

		buffer vertices;
		buffer indices;
		GLenum index_type;
	};

	struct KREOGL_EXPORT model_data {
		std::vector<mesh_data> meshes;
		std::vector<size_t> vertex_attribute_offsets;
		size_t vertex_size;
	};
}