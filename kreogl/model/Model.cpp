#include "Model.hpp"

// stl
#include <iostream>
#include <cassert>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	static void registerVertexAttribute(size_t vertexSize, size_t location, const VertexSpecification::Attribute & attribute, std::ptrdiff_t offset) noexcept {
		KREOGL_PROFILING_SCOPE;

		// register vertex attributes

		glEnableVertexAttribArray((GLuint)location);
		if (attribute.type == GL_FLOAT)
			glVertexAttribPointer((GLuint)location, (GLint)attribute.elementCount, attribute.type, GL_FALSE, (GLsizei)vertexSize, (void *)offset);
		else
			glVertexAttribIPointer((GLuint)location, (GLint)attribute.elementCount, attribute.type, (GLsizei)vertexSize, (void *)offset);
	}

	Mesh::Mesh(const MeshData & data, const ModelData & model, const VertexSpecification & vertexSpecification) noexcept {
		KREOGL_PROFILING_SCOPE;

		glBindVertexArray(vertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(data.vertices.nbElements * data.vertices.elementSize), data.vertices.data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(data.indices.nbElements * data.indices.elementSize), data.indices.data, GL_STATIC_DRAW);

		nbIndices = data.indices.nbElements;

		assert(model.vertexAttributeOffsets.size() == vertexSpecification.attributes.size());
		for (size_t location = 0; location < model.vertexAttributeOffsets.size(); ++location)
			registerVertexAttribute(model.vertexSize, location, vertexSpecification.attributes[location], model.vertexAttributeOffsets[location]);

		indexType = data.indexType;
	}

	void Mesh::draw() const noexcept {
		KREOGL_PROFILING_SCOPE;

		glBindVertexArray(vertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glDrawElements(GL_TRIANGLES, (GLsizei)nbIndices, indexType, nullptr);
	}

	Model::Model(const VertexSpecification & vertexSpecification, const ModelData & data) noexcept
	: vertexSpecification(vertexSpecification)
	{
		KREOGL_PROFILING_SCOPE;

		meshes.clear();
		for (const auto & meshData : data.meshes)
			meshes.emplace_back(meshData, data, vertexSpecification);
	}

	void Model::draw() const noexcept {
		KREOGL_PROFILING_SCOPE;

		for (const auto & mesh : meshes)
			mesh.draw();
	}
}