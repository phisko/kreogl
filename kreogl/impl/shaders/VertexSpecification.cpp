#include "VertexSpecification.hpp"

#include "kreogl/animation/AnimatedModel.hpp"

namespace kreogl {
	const VertexSpecification VertexSpecification::positionColor = { {
		{ // position
			.type = GL_FLOAT,
			.elementCount = 3
		},
		{ // color
			.type = GL_FLOAT,
			.elementCount = 3
		}
	} };

	const VertexSpecification VertexSpecification::skeletal = { {
		{ // position
			.type = GL_FLOAT,
			.elementCount = 3 },
		{ // normal
			.type = GL_FLOAT,
			.elementCount = 3
		},
		{ // texCoords
			.type = GL_FLOAT,
			.elementCount = 2
		},
		{ // boneWeights
			.type = GL_FLOAT,
			.elementCount = KREOGL_BONE_INFO_PER_VERTEX
		},
		{ // boneIDs
			.type = GL_UNSIGNED_INT,
			.elementCount = KREOGL_BONE_INFO_PER_VERTEX
		}
	} };
}