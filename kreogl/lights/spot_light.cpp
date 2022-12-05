#include "spot_light.hpp"

// glm
#include <glm/gtc/matrix_transform.hpp>

// kreogl
#include "kreogl/camera.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	static glm::vec3 get_correct_direction(const glm::vec3 & dir) noexcept {
		const auto normalized = glm::normalize(dir);
		return normalized.y == 1.f ?
			glm::vec3(.001f, .999f, 0.f) :
			normalized.y == -1.f ?
			glm::vec3(.001f, -.999f, 0.f) :
			dir;
	}

	glm::mat4 spot_light::get_light_space_matrix() const noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto light_proj = glm::perspective(glm::radians(90.f), 1.f, KREOGL_SHADOW_MAP_NEAR_PLANE, get_radius());

		const auto dir = get_correct_direction(direction);
		const auto light_view = glm::lookAt(position, position + dir, glm::vec3(0.f, 1.f, 0.f));

		return light_proj * light_view;
	}
}