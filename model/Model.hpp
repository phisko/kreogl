#pragma once

#include "impl/RAII/Buffer.hpp"
#include "impl/RAII/VertexArray.hpp"
#include "ModelData.hpp"
#include "impl/shaders/VertexSpecification.hpp"

namespace kreogl {
    struct Mesh {
        Mesh() noexcept = default;
        Mesh(const MeshData & data, const ModelData & model, const VertexSpecification & vertexSpecification) noexcept;
        void draw() const noexcept;

        VertexArray vertexArray;
        Buffer vertexBuffer;
        Buffer indexBuffer;
        size_t nbIndices;
        GLenum indexType = GL_UNSIGNED_INT;
    };

    struct Model {
        Model(const ModelData & data, const VertexSpecification & vertexSpecification) noexcept;
        void draw() const noexcept;

        std::vector<Mesh> meshes;
        const VertexSpecification & vertexSpecification;
    };
}