#include "skeletal_textured_shader.hpp"

// kreogl
#include "kreogl/impl/shaders/helpers/apply_transparency/apply_transparency_glsl.hpp"
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	namespace {
		static constexpr auto DIFFUSE_TEXTURE_INDEX = (int)gbuffer::texture::count;
		static constexpr auto SPECULAR_TEXTURE_INDEX = (int)gbuffer::texture::count + 1;
	}

	skeletal_textured_shader::skeletal_textured_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("skeletal_textured_shader");

		use_without_uniform_check();
		_glsl.texture_diffuse = DIFFUSE_TEXTURE_INDEX;
		_glsl.texture_specular = SPECULAR_TEXTURE_INDEX;
	}

	void skeletal_textured_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(skeletal_glsl::vert, GL_VERTEX_SHADER);
		add_source_file(skeletal_glsl::frag, GL_FRAGMENT_SHADER);
		add_source_file(apply_transparency_glsl::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<uniform_base *> skeletal_textured_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			&_glsl.model,
			&_glsl.view,
			&_glsl.proj,
			&_glsl.bones,
			&_glsl.has_texture,
			&_glsl.texture_diffuse,
			&_glsl.texture_specular,
			&_glsl.diffuse_color,
			&_glsl.specular_color,
			&_glsl.color,
			&_glsl.user_data
		};
	}

	void skeletal_textured_shader::draw(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto uniform_checker = use(false);
		_glsl.view = params.camera.get_view_matrix();
		_glsl.proj = params.camera.get_proj_matrix();

		for (const auto object : params.world.get_objects(vertex_specification::skeletal)) {
			_glsl.model = object->transform;
			_glsl.color = object->color;
			_glsl.user_data = object->user_data;
			uniform_checker.should_check = true;

			draw_object(*object);
		}
	}

	void skeletal_textured_shader::set_mesh_uniforms(const animated_model & model, unsigned int mesh_index) noexcept {
		KREOGL_PROFILING_SCOPE;

		bind_textures(model.textures[mesh_index]);
	}

	void skeletal_textured_shader::bind_textures(const animated_model::mesh_textures & textures) noexcept {
		KREOGL_PROFILING_SCOPE;

		// Diffuse
		const bool has_texture = !textures.diffuse_textures.empty();
		_glsl.has_texture = has_texture;
		if (has_texture) {
			glActiveTexture((GLenum)(GL_TEXTURE0 + DIFFUSE_TEXTURE_INDEX));
			glBindTexture(GL_TEXTURE_2D, *textures.diffuse_textures[0].texture);
			_glsl.diffuse_color.mark_as_used();
		}
		else
			_glsl.diffuse_color = textures.diffuse_color;

		// Specular
		if (!textures.specular_textures.empty()) {
			glActiveTexture((GLenum)(GL_TEXTURE0 + SPECULAR_TEXTURE_INDEX));
			glBindTexture(GL_TEXTURE_2D, *textures.specular_textures[0].texture);
			_glsl.specular_color.mark_as_used();
		}
		else
			_glsl.specular_color = textures.specular_color;
	}
}