#include "vertex_specification.hpp"

#include "kreogl/animation/animated_model.hpp"

namespace kreogl {
	const vertex_specification vertex_specification::position_color = { {
		{ // position
			.type = GL_FLOAT,
			.element_count = 3
		},
		{ // color
			.type = GL_FLOAT,
			.element_count = 3
		}
	} };

	const vertex_specification vertex_specification::skeletal = { {
		{ // position
			.type = GL_FLOAT,
			.element_count = 3 },
		{ // normal
			.type = GL_FLOAT,
			.element_count = 3
		},
		{ // tex_coords
			.type = GL_FLOAT,
			.element_count = 2
		},
		{ // bone_weights
			.type = GL_FLOAT,
			.element_count = KREOGL_BONE_INFO_PER_VERTEX
		},
		{ // bone_ids
			.type = GL_UNSIGNED_INT,
			.element_count = KREOGL_BONE_INFO_PER_VERTEX
		}
	} };
}