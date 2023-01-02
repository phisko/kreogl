#include "model.hpp"

// stl
#include <iostream>
#include <cassert>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	static void register_vertex_attribute(size_t vertex_size, size_t location, const kreogl::vertex_specification::attribute & attribute, std::ptrdiff_t offset) noexcept {
		KREOGL_PROFILING_SCOPE;

		// register vertex attributes

		glEnableVertexAttribArray((GLuint)location);
		if (attribute.type == GL_FLOAT)
			glVertexAttribPointer((GLuint)location, (GLint)attribute.element_count, attribute.type, GL_FALSE, (GLsizei)vertex_size, (void *)offset);
		else
			glVertexAttribIPointer((GLuint)location, (GLint)attribute.element_count, attribute.type, (GLsizei)vertex_size, (void *)offset);
	}

	mesh::mesh(const kreogl::mesh_data & data, const kreogl::model_data & model, const kreogl::vertex_specification & vertex_specification) noexcept {
		KREOGL_PROFILING_SCOPE;

		glBindVertexArray(vertex_array);

		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(data.vertices.nb_elements * data.vertices.element_size), data.vertices.data, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(data.indices.nb_elements * data.indices.element_size), data.indices.data, GL_STATIC_DRAW);

		nb_indices = data.indices.nb_elements;

		assert(model.vertex_attribute_offsets.size() == vertex_specification.attributes.size());
		for (size_t location = 0; location < model.vertex_attribute_offsets.size(); ++location)
			register_vertex_attribute(model.vertex_size, location, vertex_specification.attributes[location], model.vertex_attribute_offsets[location]);

		index_type = data.index_type;
	}

	void mesh::draw() const noexcept {
		KREOGL_PROFILING_SCOPE;

		glBindVertexArray(vertex_array);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glDrawElements(GL_TRIANGLES, (GLsizei)nb_indices, index_type, nullptr);
	}

	model::model(const kreogl::vertex_specification & vertex_specification, const kreogl::model_data & data) noexcept
		: vertex_specification(vertex_specification) {
		KREOGL_PROFILING_SCOPE;

		meshes.clear();
		for (const auto & mesh_data : data.meshes)
			meshes.emplace_back(mesh_data, data, vertex_specification);
	}

	void model::draw() const noexcept {
		KREOGL_PROFILING_SCOPE;

		for (const auto & mesh : meshes)
			mesh.draw();
	}
}