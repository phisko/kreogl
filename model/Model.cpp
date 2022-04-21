#include "Model.hpp"

#include <iostream>
#include <cassert>

namespace kreogl {
    static void registerVertexAttribute(size_t vertexSize, size_t location, const ModelData::VertexAttribute & attribute) noexcept {
        // register vertex attributes

        glEnableVertexAttribArray((GLuint)location);
        if (attribute.type == GL_FLOAT)
            glVertexAttribPointer((GLuint)location, (GLint)attribute.elementCount, attribute.type, GL_FALSE, (GLsizei)vertexSize, (void *)attribute.offset);
        else
            glVertexAttribIPointer((GLuint)location, (GLint)attribute.elementCount, attribute.type, (GLsizei)vertexSize, (void *)attribute.offset);
    }

    Mesh::Mesh(const MeshData &data, const ModelData & model) noexcept {
        glBindVertexArray(vertexArray);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(data.vertices.nbElements * data.vertices.elementSize), data.vertices.data, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(data.indices.nbElements * data.indices.elementSize), data.indices.data, GL_STATIC_DRAW);

        nbIndices = data.indices.nbElements;

        size_t location = 0;
        for (const auto & attribute : model.vertexAttributes)
            registerVertexAttribute(model.vertexSize, location++, attribute);

        indexType = data.indexType;
    }

    void Mesh::draw() const noexcept {
        glBindVertexArray(vertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glDrawElements(GL_TRIANGLES, (GLsizei)nbIndices, indexType, nullptr);
    }


    Model::Model(const ModelData &data, VertexSpecification vertexSpecification) noexcept
        : vertexSpecification(vertexSpecification)
    {
        for (const auto & meshData : data.meshes)
            meshes.emplace_back(meshData, data);
    }
}