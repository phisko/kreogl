#pragma once

#include <functional>
#include <vector>
#include <GL/glew.h>

namespace kreogl {
    struct MeshData {
        struct Buffer {
            size_t nbElements;
            size_t elementSize;
            const void * data;
        };

        Buffer vertices;
        Buffer indices;
        GLenum indexType;
    };

    struct ModelData {
        std::vector<MeshData> meshes;
        std::function<void()> release = nullptr;

        struct VertexAttribute {
            const char * name;
            size_t offset;
            GLenum type;
            size_t elementCount;
        };

        std::vector<VertexAttribute> vertexAttributes;
        size_t vertexSize;

        ~ModelData() noexcept;
    };
}