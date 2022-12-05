#pragma once

// kreogl
#include "kreogl/impl/default_constructors.hpp"
#include "kreogl/impl/raii/buffer.hpp"
#include "kreogl/impl/raii/vertex_array.hpp"
#include "kreogl/impl/shaders/vertex_specification.hpp"
#include "model_data.hpp"

namespace kreogl {
	struct KREOGL_EXPORT mesh {
		KREOGL_MOVE_ONLY(mesh);

		mesh(const kreogl::mesh_data & data, const kreogl::model_data & model, const kreogl::vertex_specification & vertex_specification) noexcept;
		void draw() const noexcept;

		kreogl::vertex_array vertex_array;
		kreogl::buffer vertex_buffer;
		kreogl::buffer index_buffer;
		size_t nb_indices;
		GLenum index_type = GL_UNSIGNED_INT;
	};

	class KREOGL_EXPORT model {
	public:
		KREOGL_DELETE_COPY(model);

		virtual ~model() noexcept = default;

		model(const kreogl::vertex_specification & vertex_specification, const kreogl::model_data & data) noexcept;
		void draw() const noexcept;

		const kreogl::vertex_specification & vertex_specification;
		std::vector<kreogl::mesh> meshes;
	};
}