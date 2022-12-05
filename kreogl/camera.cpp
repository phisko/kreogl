#include "camera.hpp"

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	camera::camera(const construction_params & params) noexcept
	: _position(params.position),
	_direction(params.direction),
	_fov(params.fov),
	_viewport(params.viewport),
	_near_plane(params.near_plane),
	_far_plane(params.far_plane)
	{}

	void camera::set_position(const glm::vec3 & position) noexcept {
		KREOGL_PROFILING_SCOPE;
		_position = position;
		_cached_view_matrix = std::nullopt;
	}

	void camera::set_direction(const glm::vec3 & direction) noexcept {
		KREOGL_PROFILING_SCOPE;
		_direction = direction;
		_cached_view_matrix = std::nullopt;
	}

	void camera::set_fov(float fov) noexcept {
		KREOGL_PROFILING_SCOPE;
		_fov = fov;
		_cached_proj_matrix = std::nullopt;
	}

	void camera::set_near_plane(float near_plane) noexcept {
		KREOGL_PROFILING_SCOPE;
		_near_plane = near_plane;
		_cached_proj_matrix = std::nullopt;
	}

	void camera::set_far_plane(float far_plane) noexcept {
		KREOGL_PROFILING_SCOPE;
		_far_plane = far_plane;
		_cached_proj_matrix = std::nullopt;
	}

	viewport & camera::get_viewport_writable() noexcept {
		KREOGL_PROFILING_SCOPE;
		_cached_proj_matrix = std::nullopt;
		return _viewport;
	}

	const glm::mat4 & camera::get_view_matrix() const noexcept {
		KREOGL_PROFILING_SCOPE;
		if (_cached_view_matrix)
			return *_cached_view_matrix;
		_cached_view_matrix = glm::lookAt(_position, _position + _direction, glm::vec3{ 0, 1, 0 });
		return *_cached_view_matrix;
	}

	const glm::mat4 & camera::get_proj_matrix() const noexcept {
		KREOGL_PROFILING_SCOPE;
		if (_cached_proj_matrix)
			return *_cached_proj_matrix;
		_cached_proj_matrix = glm::perspective(_fov, _viewport.get_aspect_ratio(), _near_plane, _far_plane);
		return *_cached_proj_matrix;
	}
}