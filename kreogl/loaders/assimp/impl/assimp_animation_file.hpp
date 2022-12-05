#pragma once

// stl
#include <memory>

// assimp
#include <assimp/Importer.hpp>

// kreogl
#include "kreogl/animation/animation_file.hpp"

namespace kreogl {
	struct KREOGL_EXPORT assimp_animation_file : animation_file {
		std::unique_ptr<Assimp::Importer> importer = nullptr;
	};
}