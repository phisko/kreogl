#pragma once

// kreogl
#include "kreogl/animation/AnimationModel.hpp"

// glm
#include <glm/glm.hpp>

// AssImp
#include <assimp/anim.h>
#include <assimp/scene.h>

namespace kreogl {
	struct AnimatedObject;

	struct AssImpAnimationModel : AnimationModel {
		const aiAnimation * assimpAnimation = nullptr;

	private: // AnimationModel implementation
		void tick(float deltaTime, AnimatedObject & object) const noexcept override;

	private:
		void incrementDeltaTime(float deltaTime, AnimatedObject & object) const noexcept;
		void updateBoneMats(const aiNode & node, float time, AnimatedObject & object, const glm::mat4 & parentTransform, bool applyMoverBehavior) const noexcept;
	};
}