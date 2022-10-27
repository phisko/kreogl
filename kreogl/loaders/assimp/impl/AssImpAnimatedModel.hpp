#pragma once

#include "kreogl/animation/AnimatedModel.hpp"
#include <assimp/Importer.hpp>
#include "AssImpAnimationModel.hpp"

namespace kreogl {
	class AssImpAnimatedModel : public AnimatedModel {
	public:
		std::unique_ptr<Assimp::Importer> importer;

		using AnimatedModel::AnimatedModel;
	};
}