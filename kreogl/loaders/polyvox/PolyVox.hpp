#pragma once

#include "kreogl/model/Model.hpp"

#include "PolyVox/RawVolume.h"
#include "PolyVox/CubicSurfaceExtractor.h"

namespace kreogl::PolyVox {
    template<typename VertexData>
    Model loadModel(::PolyVox::RawVolume<VertexData> & volume) noexcept;

    template<typename VertexData>
    Model loadModel(::PolyVox::RawVolume<VertexData> & volume, const VertexSpecification & vertexSpecification, const std::vector<size_t> & vertexAttributeOffsets) noexcept;

    template<typename Mesh>
    Model loadModel(const Mesh & mesh, const VertexSpecification & vertexSpecification, const std::vector<size_t> & vertexAttributeOffsets) noexcept;
}

#include "PolyVox.inl"