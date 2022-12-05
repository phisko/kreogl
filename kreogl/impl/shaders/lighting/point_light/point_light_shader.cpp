#include "point_light_shader.hpp"

// kreogl
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/shapes/sphere.hpp"
#include "kreogl/impl/gbuffer.hpp"
#include "kreogl/impl/raii/scoped_bind_framebuffer.hpp"
#include "kreogl/impl/raii/scoped_gl_feature.hpp"
#include "kreogl/impl/shaders/shader_pipeline.hpp"
#include "kreogl/impl/shaders/shadow_map/shadow_cube_shader.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	point_light_shader::point_light_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("point_light_shader");

#ifndef NDEBUG
		assert(_glsl.position.location == _shadow_cube_glsl.position.location);
		_shadow_cube_glsl.position.used = true;

		assert(_glsl.view_pos.location == _shadow_cube_glsl.view_pos.location);
		_shadow_cube_glsl.view_pos.used = true;
#endif

		use_without_uniform_check();

		_glsl.gposition = (int)gbuffer::texture::position;
		_glsl.gnormal = (int)gbuffer::texture::normal;
		_glsl.gdiffuse = (int)gbuffer::texture::diffuse_and_should_ignore_lighting;
		_glsl.gspecular = (int)gbuffer::texture::specular;

		_shadow_cube_glsl.shadow_map = (int)gbuffer::texture::count;
	}

	void point_light_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(position_proj_view_model_glsl::vert, GL_VERTEX_SHADER);
		add_source_file(point_light_glsl::frag, GL_FRAGMENT_SHADER);
		add_source_file(sample_shadow_cube_glsl::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<uniform_base *> point_light_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			// position_proj_view_model_glsl
			&_ppvm_glsl.proj,
			&_ppvm_glsl.view,
			&_ppvm_glsl.model,
			// point_light_glsl
			&_glsl.gposition,
			&_glsl.gnormal,
			&_glsl.gdiffuse,
			&_glsl.gspecular,
			&_glsl.view_pos,
			&_glsl.screen_size,
			&_glsl.color,
			&_glsl.position,
			&_glsl.diffuse_strength,
			&_glsl.specular_strength,
			&_glsl.attenuation_constant,
			&_glsl.attenuation_linear,
			&_glsl.attenuation_quadratic,
			// sample_shadow_cube_glsl
			&_shadow_cube_glsl.shadow_map,
			&_shadow_cube_glsl.position,
			&_shadow_cube_glsl.view_pos,
			&_shadow_cube_glsl.far_plane,
			&_shadow_cube_glsl.min_bias,
			&_shadow_cube_glsl.max_bias,
		};
	}

	void point_light_shader::draw(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		use_without_uniform_check();

		const scoped_gl_feature cull(GL_CULL_FACE);
		const scoped_gl_feature blend(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		_glsl.screen_size = params.camera.get_viewport().get_resolution();
		_glsl.view_pos = params.camera.get_position();
		assert(_shadow_cube_glsl.view_pos.location == _glsl.view_pos.location); // If this fails, we need to explicitly set both uniforms

		glActiveTexture((GLenum)(GL_TEXTURE0 + (int)gbuffer::texture::count));

		for (const auto light : params.world.get_point_lights()) {
			if (light->cast_shadows)
				update_shadow_map(*light, params);

			const auto radius = light->get_radius();
			if (glm::length(params.camera.get_position() - light->position) < radius)
				glCullFace(GL_BACK);
			else
				glCullFace(GL_FRONT);

			const auto uniform_checker = use();

			_ppvm_glsl.proj = params.camera.get_proj_matrix();
			_ppvm_glsl.view = params.camera.get_view_matrix();

			glm::mat4 model(1.f);
			model = glm::translate(model, light->position);
			model = glm::scale(model, glm::vec3(radius));
			_ppvm_glsl.model = model;

			_glsl.color = light->color;
			_glsl.position = light->position;
			_glsl.diffuse_strength = light->diffuse_strength;
			_glsl.specular_strength = light->specular_strength;
			_glsl.attenuation_constant = light->attenuation_constant;
			_glsl.attenuation_linear = light->attenuation_linear;
			_glsl.attenuation_quadratic = light->attenuation_quadratic;
			_shadow_cube_glsl.far_plane = light->get_radius();
			_shadow_cube_glsl.min_bias = light->shadow_map_min_bias;
			_shadow_cube_glsl.max_bias = light->shadow_map_max_bias;

			glBindTexture(GL_TEXTURE_CUBE_MAP, light->shadow_cube.texture);
			kreogl::shapes::draw_sphere();
		}

		glCullFace(GL_BACK);
	}

	void point_light_shader::update_shadow_map(const point_light & light, const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		const scoped_bind_framebuffer bind(light.shadow_cube.frame_buffer);
		glClear(GL_DEPTH_BUFFER_BIT);

		const auto shaders = params.shader_pipeline.get_shaders(shader_step::shadow_cube);
		if (!shaders)
			return;

		for (const auto shader : *shaders) {
			const auto shadow_cube_shader = static_cast<kreogl::shadow_cube_shader *>(shader);
			shadow_cube_shader->draw(light, params);
		}
	}
}