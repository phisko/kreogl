#pragma once

#include <glm/glm.hpp>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>

namespace kreogl {
	KREOGL_EXPORT glm::mat4 toglm(const aiMatrix4x4 & mat) noexcept;
	KREOGL_EXPORT glm::vec3 toglm(const aiVector3D & vec) noexcept;
	KREOGL_EXPORT glm::quat toglm(const aiQuaternion & quat) noexcept;
}