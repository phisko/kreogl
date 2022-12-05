#pragma once

// assimp
#include <assimp/Importer.hpp>

// kreogl
#include "kreogl/animation/animated_model.hpp"
#include "assimp_animation_model.hpp"

namespace kreogl {
	class KREOGL_EXPORT assimp_animated_model : public animated_model {
	public:
		std::unique_ptr<Assimp::Importer> importer;

		using animated_model::animated_model;
	};
}