#pragma once

// stl
#include <optional>

// glm
#include <glm/gtc/matrix_transform.hpp>

// kreogl
#include "impl/viewport.hpp"

namespace kreogl {
	class KREOGL_EXPORT camera {
	public:
		struct construction_params {
			glm::vec3 position = { 0, 0, 0 };
			glm::vec3 direction = { 0, 0, 1 };
			float fov = 1.f;
			kreogl::viewport::construction_params viewport = {};
			float near_plane = 1.f;
			float far_plane = 1000.f;
		};
		camera(const construction_params & params) noexcept;

	public:
		const glm::vec3 & get_position() const noexcept { return _position; }
		void set_position(const glm::vec3 & position) noexcept;

		const glm::vec3 & get_direction() const noexcept { return _direction; }
		void set_direction(const glm::vec3 & direction) noexcept;

		float get_fov() const noexcept { return _fov; }
		void set_fov(float fov) noexcept;

		const viewport & get_viewport() const noexcept { return _viewport; }
		viewport & get_viewport_writable() noexcept;

		float get_near_plane() const noexcept { return _near_plane; }
		void set_near_plane(float near_plane) noexcept;

		float get_far_plane() const noexcept { return _far_plane; }
		void set_far_plane(float far_plane) noexcept;

		const glm::mat4 & get_view_matrix() const noexcept;
		const glm::mat4 & get_proj_matrix() const noexcept;

	private:
		glm::vec3 _position;
		glm::vec3 _direction;
		float _fov;
		viewport _viewport;

		float _near_plane;
		float _far_plane;

		mutable std::optional<glm::mat4> _cached_view_matrix;
		mutable std::optional<glm::mat4> _cached_proj_matrix;
	};
}