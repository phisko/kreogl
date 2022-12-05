#pragma once

// glm
#include <glm/glm.hpp>

// assimp
#include <assimp/anim.h>
#include <assimp/scene.h>

// kreogl
#include "kreogl/animation/animation_model.hpp"

namespace kreogl {
	struct animated_object;

	struct KREOGL_EXPORT assimp_animation_model : animation_model {
		const aiAnimation * assimp_animation = nullptr;

	private: // animation_model implementation
		void tick(float delta_time, animated_object & object) const noexcept override;

	private:
		void increment_delta_time(float delta_time, animated_object & object) const noexcept;
		void update_bone_mats(const aiNode & node, float time, animated_object & object, const glm::mat4 & parent_transform, bool apply_mover_behavior) const noexcept;
	};
}