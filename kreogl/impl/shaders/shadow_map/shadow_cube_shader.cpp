#include "shadow_cube_shader.hpp"

// kreogl
#include "kreogl/camera.hpp"
#include "kreogl/impl/raii/scoped_bind_framebuffer.hpp"
#include "kreogl/impl/raii/scoped_gl_feature.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	std::vector<uniform_base *> shadow_cube_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		std::vector<uniform_base *> ret{
			&_glsl.light_pos,
			&_glsl.far_plane
		};

		for (auto & uniform : _glsl.light_space_matrices)
			ret.push_back(&uniform);

		return ret;
	}

	void shadow_cube_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(shadow_cube_glsl::geom, GL_GEOMETRY_SHADER);
		add_source_file(shadow_cube_glsl::frag, GL_FRAGMENT_SHADER);
	}

	void shadow_cube_shader::draw(const point_light & light, const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		light.shadow_cube.set_size(light.shadow_map_size);

		glViewport(0, 0, light.shadow_cube.get_size(), light.shadow_cube.get_size());
		glCullFace(GL_FRONT);

		const scoped_bind_framebuffer bound(light.shadow_cube.frame_buffer);
		const scoped_gl_feature depth(GL_DEPTH_TEST);

		use_without_uniform_check(); // We'd need to pass the checker to draw_objects

		static struct {
			glm::vec3 target;
			glm::vec3 up;
		} directions[] = {
			{ { 1.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
			{ { -1.0f, 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
			{ { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
			{ { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, -1.0f } },
			{ { 0.0f, 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } },
			{ { 0.0f, 0.0f, -1.0f }, { 0.0f, -1.0f, 0.0f } }
		};

		const auto radius = light.get_radius();
		const auto proj = glm::perspective(glm::radians(90.f), (float)light.shadow_map_size / (float)light.shadow_map_size, KREOGL_SHADOW_MAP_NEAR_PLANE, radius);
		for (unsigned int i = 0; i < 6; ++i) {
			const auto view = glm::lookAt(light.position, light.position + directions[i].target, directions[i].up);
			_glsl.light_space_matrices[i] = proj * view;
		}

		_glsl.light_pos = light.position;
		_glsl.far_plane = radius;

		draw_objects(params);

		const auto & viewport = params.camera.get_viewport();
		const auto & resolution = viewport.get_resolution();
		glViewport(0, 0, resolution.x, resolution.y);
		glCullFace(GL_BACK);
	}
}