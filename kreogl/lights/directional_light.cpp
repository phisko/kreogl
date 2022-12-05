#include "directional_light.hpp"

// stl
#include <array>

// kreogl
#include "kreogl/camera.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	static glm::vec3 get_correct_direction(const glm::vec3 & dir) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto normalized = glm::normalize(dir);
		return normalized.y == 1.f ?
			glm::vec3(.001f, .999f, 0.f) :
			normalized.y == -1.f ?
			glm::vec3(.001f, -.999f, 0.f) :
			dir;
	}

	struct cascade_bounds {
		static constexpr auto corner_count = 8;
		std::array<glm::vec4, corner_count> corners;
		glm::vec3 center;
	};

	static cascade_bounds get_cascade_bounds_world_space(const glm::mat4 & proj, const glm::mat4 & view) noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto inv = glm::inverse(proj * view);

		const glm::vec3 ndc_corners[] = {
			{ -1.f, -1.f, -1.f }, // left bottom back
			{ 1.f, -1.f, -1.f }, // right bottom back
			{ -1.f, 1.f, -1.f, }, // left top back
			{ 1.f, 1.f, -1.f }, // right top back
			{ -1.f, -1.f, 1.f }, // left bottom front
			{ 1.f, -1.f, 1.f }, // right bottom front
			{ -1.f, 1.f, 1.f }, // left top front
			{ 1.f, 1.f, 1.f }, // right top front
		};
		static_assert(std::extent_v<decltype(ndc_corners)> == cascade_bounds::corner_count);

		cascade_bounds bounds;
		for (unsigned int i = 0; i < cascade_bounds::corner_count; ++i) {
			const auto pt = inv * glm::vec4(ndc_corners[i], 1.f);
			bounds.corners[i] = pt / pt.w;
		}

		bounds.center = glm::vec3(0, 0, 0);
		for (const auto & v : bounds.corners)
			bounds.center += glm::vec3(v);
		bounds.center /= cascade_bounds::corner_count;

		return bounds;
	}

	// based on https://learnopengl.com/Guest-Articles/2021/CSM
	glm::mat4 directional_light::get_light_space_matrix_for_cascade(const draw_params & params, size_t index) const noexcept {
		KREOGL_PROFILING_SCOPE;

		const auto near_plane = index == 0 ? params.camera.get_near_plane() : std::max(params.camera.get_near_plane(), cascade_ends[index - 1]);
		const auto far_plane = std::min(params.camera.get_far_plane(), cascade_ends[index]);

		const auto proj = glm::perspective(params.camera.get_fov(), params.camera.get_viewport().get_aspect_ratio(), near_plane, far_plane);

		const auto cascade_bounds_world_space = get_cascade_bounds_world_space(proj, params.camera.get_view_matrix());

		const auto dir = get_correct_direction(direction);
		const auto light_view = glm::lookAt(cascade_bounds_world_space.center - glm::normalize(dir), cascade_bounds_world_space.center, { 0.f, 1.f, 0.f });

		glm::vec3 min(std::numeric_limits<float>::max());
		glm::vec3 max(std::numeric_limits<float>::min());

		for (const auto & world_pos : cascade_bounds_world_space.corners) {
			const auto light_pos = light_view * world_pos;
			min = glm::min(min, glm::vec3(light_pos));
			max = glm::max(max, glm::vec3(light_pos));
		}

		const auto largest_extent = glm::max(glm::abs(min), glm::abs(max));
		const auto light_proj = glm::ortho(-largest_extent.x, largest_extent.x, -largest_extent.y, largest_extent.y, -largest_extent.z - shadow_caster_max_distance, largest_extent.z + shadow_caster_max_distance);

		return light_proj * light_view;
	}
}