#pragma once

// polyvox
#include "PolyVox/RawVolume.h"
#include "PolyVox/CubicSurfaceExtractor.h"

// kreogl
#include "kreogl/model/Model.hpp"

namespace kreogl::PolyVox {
	template<typename VertexData>
	Model loadModel(::PolyVox::RawVolume<VertexData> & volume) noexcept;

	template<typename VertexData>
	Model loadModel(::PolyVox::RawVolume<VertexData> & volume, const VertexSpecification & vertexSpecification, const std::vector<size_t> & vertexAttributeOffsets) noexcept;

	template<typename Mesh>
	Model loadModel(const Mesh & mesh, const VertexSpecification & vertexSpecification, const std::vector<size_t> & vertexAttributeOffsets) noexcept;
}

#include "PolyVox.inl"