#include "polyvox.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl::polyvox {
	template<typename vertex_data>
	model load_model(::PolyVox::RawVolume<vertex_data> & volume) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto encoded_mesh = ::PolyVox::extractCubicMesh(&volume, volume.getEnclosingRegion());
		const auto mesh = ::PolyVox::decodeMesh(encoded_mesh);

		using vertex_type = typename decltype(mesh)::VertexType;
		return load_model(mesh, kreogl::vertex_specification::position_color, { offsetof(vertex_type, position), offsetof(vertex_type, data.color) });
	}

	template<typename vertex_data>
	model load_model(::PolyVox::RawVolume<vertex_data> & volume, const vertex_specification & vertex_specification, const std::vector<size_t> & vertex_attribute_offsets) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto encoded_mesh = ::PolyVox::extractCubicMesh(&volume, volume.getEnclosingRegion());
		const auto mesh = ::PolyVox::decodeMesh(encoded_mesh);
		return load_model(mesh, vertex_specification, vertex_attribute_offsets);
	}

	template<typename Mesh>
	model load_model(const Mesh & mesh, const vertex_specification & vertex_specification, const std::vector<size_t> & vertex_attribute_offsets) noexcept {
		KREOGL_PROFILING_SCOPE;

		using vertex_type = typename Mesh::VertexType;
		using index_type = typename Mesh::IndexType;
		static_assert(std::is_same_v<index_type, unsigned int>);

		const kreogl::mesh_data mesh_data{
			.vertices = {
				.nb_elements = mesh.getNoOfVertices(),
				.element_size = sizeof(vertex_type),
				.data = mesh.getRawVertexData()
			},
			.indices = {
				.nb_elements = mesh.getNoOfIndices(),
				.element_size = sizeof(index_type),
				.data = mesh.getRawIndexData()
			},
			.index_type = GL_UNSIGNED_INT
		};

		const kreogl::model_data model_data{
			.meshes = { mesh_data },
			.vertex_attribute_offsets = vertex_attribute_offsets,
			.vertex_size = sizeof(vertex_type)
		};

		return { vertex_specification, model_data };
	}
}