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

	class Model {
	public:
		virtual ~Model() noexcept = default;

		Model(const VertexSpecification & vertexSpecification, const ModelData & data) noexcept;
		void draw() const noexcept;

		const VertexSpecification & vertexSpecification;
		std::vector<Mesh> meshes;

		Model(const Model &) noexcept = default;
		Model(Model &&) noexcept = default;
		Model & operator=(const Model &) noexcept = default;
		Model & operator=(Model &&) noexcept = default;
	};
}