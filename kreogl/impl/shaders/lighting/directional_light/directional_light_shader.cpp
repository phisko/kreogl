#include "directional_light_shader.hpp"

// kreogl
#include "kreogl/impl/shaders/helpers/quad/quad_glsl.hpp"
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/gbuffer.hpp"
#include "kreogl/impl/raii/scoped_bind_framebuffer.hpp"
#include "kreogl/impl/raii/scoped_gl_feature.hpp"
#include "kreogl/impl/shaders/shader_pipeline.hpp"
#include "kreogl/impl/shaders/shadow_map/shadow_map_shader.hpp"
#include "kreogl/impl/shapes/quad.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	directional_light_shader::directional_light_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("directional_light_shader");

		use_without_uniform_check();

		_glsl.gposition = (int)gbuffer::texture::position;
		_glsl.gnormal = (int)gbuffer::texture::normal;
		_glsl.gdiffuse = (int)gbuffer::texture::diffuse_and_should_ignore_lighting;
		_glsl.gspecular = (int)gbuffer::texture::specular;

		for (size_t i = 0; i < KREOGL_MAX_CSM_COUNT; ++i)
			_csm_glsl.shadow_map[i] = (int)gbuffer::texture::count + i;
	}

	void directional_light_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(quad_glsl::vert, GL_VERTEX_SHADER);
		add_source_file(directional_light_glsl::frag, GL_FRAGMENT_SHADER);
		add_source_file(sample_cascaded_shadow_map_glsl::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<uniform_base *> directional_light_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		std::vector<uniform_base *> ret{
			// directional_light_glsl
			&_glsl.gposition,
			&_glsl.gnormal,
			&_glsl.gdiffuse,
			&_glsl.gspecular,
			&_glsl.view_pos,
			&_glsl.screen_size,
			&_glsl.color,
			&_glsl.direction,
			&_glsl.ambient_strength,
			&_glsl.diffuse_strength,
			&_glsl.specular_strength,
			// sample_cascaded_shadow_map_glsl
			&_csm_glsl.cascade_count,
			&_csm_glsl.min_bias,
			&_csm_glsl.max_bias,
			&_csm_glsl.pcf_samples,
			&_csm_glsl.view
		};

		for (auto & uniform : _csm_glsl.shadow_map)
			ret.push_back(&uniform);

		for (auto & uniform : _csm_glsl.light_space_matrix)
			ret.push_back(&uniform);

		for (auto & uniform : _csm_glsl.cascade_end)
			ret.push_back(&uniform);

		return ret;
	}

	void directional_light_shader::draw(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		use_without_uniform_check();

		const scoped_gl_feature blend(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		_csm_glsl.view = params.camera.get_view_matrix();

		_glsl.view_pos = params.camera.get_position();
		_glsl.screen_size = params.camera.get_viewport().get_resolution();

		for (const auto light : params.world.get_directional_lights()) {
			if (light->cast_shadows)
				update_shadow_map(*light, params);

			use_without_uniform_check(); // All cascades might not be used
			_glsl.color = light->color;
			_glsl.direction = light->direction;
			_glsl.ambient_strength = light->ambient_strength;
			_glsl.diffuse_strength = light->diffuse_strength;
			_glsl.specular_strength = light->specular_strength;
			_csm_glsl.pcf_samples = light->shadow_pcf_samples;
			_csm_glsl.max_bias = light->shadow_map_max_bias;
			_csm_glsl.min_bias = light->shadow_map_min_bias;

			for (size_t i = 0; i < light->cascade_ends.size(); ++i) {
				glActiveTexture((GLenum)(GL_TEXTURE0 + (int)gbuffer::texture::count + i));
				glBindTexture(GL_TEXTURE_2D, light->cascaded_shadow_map.textures[i]);
				_csm_glsl.light_space_matrix[i] = light->get_light_space_matrix_for_cascade(params, i);
				_csm_glsl.cascade_end[i] = light->cascade_ends[i];
			}
			_csm_glsl.cascade_count = (int)light->cascade_ends.size();

			kreogl::shapes::draw_quad();
		}
	}

	void directional_light_shader::update_shadow_map(const directional_light & light, const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		const scoped_bind_framebuffer bind(light.cascaded_shadow_map.frame_buffer);
		for (const auto & texture : light.cascaded_shadow_map.textures) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
			glClear(GL_DEPTH_BUFFER_BIT);
		}

		const auto shaders = params.shader_pipeline.get_shaders(shader_step::shadow_map);
		if (!shaders)
			return;

		for (const auto shader : *shaders) {
			const auto shadow_map_shader = static_cast<kreogl::shadow_map_shader *>(shader);
			shadow_map_shader->draw(light, params);
		}
	}
}