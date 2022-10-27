#pragma once

#include <glm/glm.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>

namespace kreogl {
	glm::mat4 toglm(const aiMatrix4x4 & mat) noexcept;
	glm::vec3 toglm(const aiVector3D & vec) noexcept;
	glm::quat toglm(const aiQuaternion & quat) noexcept;
}