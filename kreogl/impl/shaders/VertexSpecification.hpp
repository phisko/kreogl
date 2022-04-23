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

        static const VertexSpecification positionColor;
    };
}
