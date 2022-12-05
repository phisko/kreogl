#include "volumetric_directional_light_shader.hpp"

// kreogl
#include "kreogl/impl/shaders/helpers/quad/quad_glsl.hpp"
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/gbuffer.hpp"
#include "kreogl/impl/raii/scoped_gl_feature.hpp"
#include "kreogl/impl/shapes/quad.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	volumetric_directional_light_shader::volumetric_directional_light_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("volumetric_directional_light_shader");

		use_without_uniform_check();

		_glsl.gposition = (int)gbuffer::texture::position;
		_glsl.gdiffuse = (int)gbuffer::texture::diffuse_and_should_ignore_lighting;

		for (size_t i = 0; i < KREOGL_MAX_CSM_COUNT; ++i)
			_csm_glsl.shadow_map[i] = (int)gbuffer::texture::count + i;
	}

	void volumetric_directional_light_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(quad_glsl::vert, GL_VERTEX_SHADER);
		add_source_file(volumetric_lighting_glsl::frag, GL_FRAGMENT_SHADER);
		add_source_file(directional_light_glsl::get_direction::frag, GL_FRAGMENT_SHADER);
		add_source_file(sample_cascaded_shadow_map_glsl::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<uniform_base *> volumetric_directional_light_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		std::vector<uniform_base *> ret{
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
			// directional_light_glsl::get_direction
			&_dir_light_glsl.direction,
			// sample_cascaded_shadow_map_glsl
			&_csm_glsl.cascade_count,
			&_csm_glsl.view,
			// Unused
			// &_csm_glsl.min_bias,
			// &_csm_glsl.max_bias,
			// &_csm_glsl.pcf_samples,
		};

		for (auto & uniform : _csm_glsl.shadow_map)
			ret.push_back(&uniform);

		for (auto & uniform : _csm_glsl.light_space_matrix)
			ret.push_back(&uniform);

		for (auto & uniform : _csm_glsl.cascade_end)
			ret.push_back(&uniform);

		return ret;
	}

	void volumetric_directional_light_shader::draw(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto uniform_checker = use(false);

		const scoped_gl_feature blend(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		_csm_glsl.view = params.camera.get_view_matrix();
		_glsl.inverse_view = glm::inverse(params.camera.get_view_matrix());
		_glsl.inverse_proj = glm::inverse(params.camera.get_proj_matrix());
		_glsl.view_pos = params.camera.get_position();
		_glsl.screen_size = params.camera.get_viewport().get_resolution();

		for (const auto light : params.world.get_directional_lights()) {
			if (light->volumetric_lighting == std::nullopt)
				continue;

			_glsl.scattering = light->volumetric_lighting->scattering;
			_glsl.nb_steps = light->volumetric_lighting->nb_steps;
			_glsl.default_step_length = light->volumetric_lighting->default_step_length;
			_glsl.intensity = light->volumetric_lighting->intensity;

			_glsl.color = light->color;
			_dir_light_glsl.direction = light->direction;

			for (size_t i = 0; i < light->cascade_ends.size(); ++i) {
				glActiveTexture((GLenum)(GL_TEXTURE0 + (int)gbuffer::texture::count + i));
				glBindTexture(GL_TEXTURE_2D, light->cascaded_shadow_map.textures[i]);
				_csm_glsl.light_space_matrix[i] = light->get_light_space_matrix_for_cascade(params, i);
				_csm_glsl.cascade_end[i] = light->cascade_ends[i];
			}
			_csm_glsl.cascade_count = (int)light->cascade_ends.size();

			uniform_checker.should_check = true;
			kreogl::shapes::draw_quad();
		}
	}
}