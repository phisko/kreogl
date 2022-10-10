#include "PolyVox.hpp"

namespace kreogl::PolyVox {
    template<typename VertexData>
    Model loadModel(::PolyVox::RawVolume<VertexData> & volume) noexcept {
        const auto encodedMesh = ::PolyVox::extractCubicMesh(&volume, volume.getEnclosingRegion());
        const auto mesh = ::PolyVox::decodeMesh(encodedMesh);

        using VertexType = typename decltype(mesh)::VertexType;
        return loadModel(mesh, kreogl::VertexSpecification::positionColor, { offsetof(VertexType, position), offsetof(VertexType, data.color) });
    }

    template<typename VertexData>
    Model loadModel(::PolyVox::RawVolume<VertexData> & volume, const VertexSpecification & vertexSpecification, const std::vector<size_t> & vertexAttributeOffsets) noexcept {
        const auto encodedMesh = ::PolyVox::extractCubicMesh(&volume, volume.getEnclosingRegion());
        const auto mesh = ::PolyVox::decodeMesh(encodedMesh);
        return loadModel(mesh, vertexSpecification, vertexAttributeOffsets);
    }

    template<typename Mesh>
    Model loadModel(const Mesh & mesh, const VertexSpecification & vertexSpecification, const std::vector<size_t> & vertexAttributeOffsets) noexcept {
        using VertexType = typename Mesh::VertexType;
        using IndexType = typename Mesh::IndexType;
        static_assert(std::is_same_v<IndexType, unsigned int>);

        const kreogl::MeshData meshData {
            .vertices = {
                .nbElements = mesh.getNoOfVertices(),
                .elementSize = sizeof(VertexType),
                .data = mesh.getRawVertexData()
            },
            .indices = {
                .nbElements = mesh.getNoOfIndices(),
                .elementSize = sizeof(IndexType),
                .data = mesh.getRawIndexData()
            },
            .indexType = GL_UNSIGNED_INT
        };

        const kreogl::ModelData modelData {
            .meshes = { meshData },
            .vertexAttributeOffsets = vertexAttributeOffsets,
            .vertexSize = sizeof(VertexType)
        };

        return { vertexSpecification, modelData };
    }
}