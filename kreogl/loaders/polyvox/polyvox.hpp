#pragma once

// polyvox
#include "PolyVox/RawVolume.h"
#include "PolyVox/CubicSurfaceExtractor.h"

// kreogl
#include "kreogl/model/model.hpp"

namespace kreogl::polyvox {
	template<typename vertex_data>
	model load_model(::PolyVox::RawVolume<vertex_data> & volume) noexcept;

	template<typename vertex_data>
	model load_model(::PolyVox::RawVolume<vertex_data> & volume, const vertex_specification & vertex_specification, const std::vector<size_t> & vertex_attribute_offsets) noexcept;

	template<typename Mesh>
	model load_model(const Mesh & mesh, const vertex_specification & vertex_specification, const std::vector<size_t> & vertex_attribute_offsets) noexcept;
}

#include "polyvox.inl"