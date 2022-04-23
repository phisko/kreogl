#include "VertexSpecification.hpp"

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
}