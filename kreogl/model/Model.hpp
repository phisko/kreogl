#pragma once

#include "kreogl/impl/RAII/Buffer.hpp"
#include "kreogl/impl/RAII/VertexArray.hpp"
#include "kreogl/impl/shaders/VertexSpecification.hpp"
#include "ModelData.hpp"

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