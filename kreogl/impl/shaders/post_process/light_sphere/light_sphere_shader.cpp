#include "light_sphere_shader.hpp"

// kreogl
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/shapes/sphere.hpp"
#include "kreogl/impl/raii/scoped_gl_feature.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	light_sphere_shader::light_sphere_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("light_sphere_shader");
	}

	void light_sphere_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(position_proj_view_model_glsl::vert, GL_VERTEX_SHADER);
		add_source_file(color_glsl::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<uniform_base *> light_sphere_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			&_glsl.proj,
			&_glsl.view,
			&_glsl.model,
			&_color_glsl.color
		};
	}

	void light_sphere_shader::draw(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		const scoped_gl_feature depth(GL_DEPTH_TEST);
		const scoped_gl_feature cull(GL_CULL_FACE);

		// Not sure why we need to cull the front face here, logic would suggest the back one
		// Perhaps we're drawing the sphere "backwards"?
		glCullFace(GL_FRONT);

		auto uniform_checker = use(false);

		_glsl.proj = params.camera.get_proj_matrix();
		_glsl.view = params.camera.get_view_matrix();

		for (const auto light : params.world.get_directional_lights()) {
			uniform_checker.should_check = true;
			const auto pos = params.camera.get_position() - light->direction * light->light_sphere_distance;
			draw_light(*light, pos);
		}

		for (const auto light : params.world.get_point_lights()) {
			uniform_checker.should_check = true;
			draw_light(*light, light->position);
		}

		for (const auto light : params.world.get_spot_lights()) {
			uniform_checker.should_check = true;
			const auto light_to_camera = light->position - params.camera.get_position();
			const bool towards_camera = glm::dot(light_to_camera, light->direction) < 0;
			if (towards_camera)
				draw_light(*light, light->position);
		}

		glCullFace(GL_BACK);
	}

	void light_sphere_shader::draw_light(const light & light, const glm::vec3 & pos) noexcept {
		KREOGL_PROFILING_SCOPE;

		glm::mat4 model{ 1.f };
		model = glm::translate(model, pos);
		model = glm::scale(model, glm::vec3(light.light_sphere_size));

		_glsl.model = model;
		_color_glsl.color = light.color;

		shapes::draw_sphere();
	}
}