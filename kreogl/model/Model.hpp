#pragma once

// kreogl
#include "kreogl/impl/DefaultConstructors.hpp"
#include "kreogl/impl/RAII/Buffer.hpp"
#include "kreogl/impl/RAII/VertexArray.hpp"
#include "kreogl/impl/shaders/VertexSpecification.hpp"
#include "ModelData.hpp"

namespace kreogl {
	struct KREOGL_EXPORT Mesh {
		KREOGL_MOVE_ONLY(Mesh);

		Mesh(const MeshData & data, const ModelData & model, const VertexSpecification & vertexSpecification) noexcept;
		void draw() const noexcept;

		VertexArray vertexArray;
		Buffer vertexBuffer;
		Buffer indexBuffer;
		size_t nbIndices;
		GLenum indexType = GL_UNSIGNED_INT;
	};

	class KREOGL_EXPORT Model {
	public:
		KREOGL_DELETE_COPY(Model);

		virtual ~Model() noexcept = default;

		Model(const VertexSpecification & vertexSpecification, const ModelData & data) noexcept;
		void draw() const noexcept;

		const VertexSpecification & vertexSpecification;
		std::vector<Mesh> meshes;
	};
}