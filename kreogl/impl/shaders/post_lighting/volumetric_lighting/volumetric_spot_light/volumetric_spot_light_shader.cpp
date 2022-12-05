#include "volumetric_spot_light_shader.hpp"

// kreogl
#include "kreogl/impl/shaders/helpers/quad/quad_glsl.hpp"
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/gbuffer.hpp"
#include "kreogl/impl/raii/scoped_gl_feature.hpp"
#include "kreogl/impl/shapes/quad.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	volumetric_spot_light_shader::volumetric_spot_light_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("volumetric_spot_light_shader");

		use_without_uniform_check();

		_glsl.gposition = (int)gbuffer::texture::position;
		_glsl.gdiffuse = (int)gbuffer::texture::diffuse_and_should_ignore_lighting;
		_shadow_map_glsl.shadow_map = (int)gbuffer::texture::count;
	}

	void volumetric_spot_light_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(quad_glsl::vert, GL_VERTEX_SHADER);
		add_source_file(volumetric_lighting_glsl::frag, GL_FRAGMENT_SHADER);
		add_source_file(point_light_glsl::get_direction::frag, GL_FRAGMENT_SHADER);
		add_source_file(sample_shadow_map_glsl::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<uniform_base *> volumetric_spot_light_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			// volumetric_lighting_glsl
			&_glsl.gposition,
			&_glsl.gdiffuse,
			&_glsl.scattering,
			&_glsl.nb_steps,
			&_glsl.default_step_length,
			&_glsl.intensity,
			&_glsl.inverse_view,
			&_glsl.inverse_proj,
			&_glsl.view_pos,
			&_glsl.screen_size,
			&_glsl.color,
			// spot_light_glsl::get_direction
			&_point_light_glsl.position,
			// sample_shadow_cube_glsl
			&_shadow_map_glsl.shadow_map,
			&_shadow_map_glsl.light_space_matrix,
			// Unused
			// &_shadow_map_glsl.min_bias,
			// &_shadow_map_glsl.max_bias,
			// &_shadow_map_glsl.pcf_samples,
		};
	}

	void volumetric_spot_light_shader::draw(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto uniform_checker = use(false);

		const scoped_gl_feature blend(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glActiveTexture((GLenum)(GL_TEXTURE0 + (int)gbuffer::texture::count));

		_glsl.inverse_view = glm::inverse(params.camera.get_view_matrix());
		_glsl.inverse_proj = glm::inverse(params.camera.get_proj_matrix());
		_glsl.view_pos = params.camera.get_position();
		_glsl.screen_size = params.camera.get_viewport().get_resolution();

		for (const auto light : params.world.get_spot_lights()) {
			if (light->volumetric_lighting == std::nullopt)
				continue;

			_glsl.scattering = light->volumetric_lighting->scattering;
			_glsl.nb_steps = light->volumetric_lighting->nb_steps;
			_glsl.default_step_length = light->volumetric_lighting->default_step_length;
			_glsl.intensity = light->volumetric_lighting->intensity;

			_glsl.color = light->color;
			_point_light_glsl.position = light->position;

			glBindTexture(GL_TEXTURE_2D, light->shadow_map.texture);
			_shadow_map_glsl.light_space_matrix = light->get_light_space_matrix();

			uniform_checker.should_check = true;
			kreogl::shapes::draw_quad();
		}
	}
}