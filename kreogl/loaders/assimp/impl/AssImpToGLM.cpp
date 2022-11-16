#include "AssImpToGLM.hpp"

// glm
#include <glm/gtc/type_ptr.hpp>

namespace kreogl {
	glm::mat4 toglm(const aiMatrix4x4 & mat) noexcept {
		return glm::transpose(glm::make_mat4(&mat.a1));
	}

	glm::vec3 toglm(const aiVector3D & vec) noexcept {
		return { vec.x, vec.y, vec.z };
	}

	glm::quat toglm(const aiQuaternion & quat) noexcept {
		return { quat.w, quat.x, quat.y, quat.z };
	}
}