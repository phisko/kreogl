#pragma once

// stl
#include <memory>

// assimp
#include <assimp/Importer.hpp>

// kreogl
#include "kreogl/animation/AnimationFile.hpp"

namespace kreogl {
	struct KREOGL_EXPORT AssImpAnimationFile : AnimationFile {
		std::unique_ptr<Assimp::Importer> importer = nullptr;
	};
}