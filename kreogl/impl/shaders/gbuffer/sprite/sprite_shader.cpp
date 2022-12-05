#include "sprite_shader.hpp"

// kreogl
#include "kreogl/impl/shaders/helpers/apply_transparency/apply_transparency_glsl.hpp"
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/shapes/textured_quad.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	sprite_shader::sprite_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("sprite_shader");

		use_without_uniform_check();
		_glsl.tex = (int)gbuffer::texture::count;
	}

	void sprite_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(sprite_glsl::vert, GL_VERTEX_SHADER);
		add_source_file(sprite_glsl::frag, GL_FRAGMENT_SHADER);
		add_source_file(apply_transparency_glsl::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<uniform_base *> sprite_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			&_glsl.proj,
			&_glsl.view,
			&_glsl.model,
			&_glsl.view_pos,
			&_glsl.tex,
			&_glsl.color,
			&_glsl.user_data
		};
	}

	void sprite_shader::draw(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto uniform_checker = use(false);

		_glsl.view_pos = params.camera.get_position();
		glActiveTexture((GLenum)(GL_TEXTURE0 + (int)gbuffer::texture::count));

		_glsl.view = glm::mat4{ 1.f };
		_glsl.proj = glm::mat4{ 1.f };
		for (const auto sprite : params.world.get_sprite_2ds()) {
			uniform_checker.should_check = true;
			draw(params, *sprite);
		}

		_glsl.view = params.camera.get_view_matrix();
		_glsl.proj = params.camera.get_proj_matrix();
		for (const auto sprite : params.world.get_sprite_3ds()) {
			uniform_checker.should_check = true;
			draw(params, *sprite);
		}
	}

	void sprite_shader::draw(const draw_params & params, const sprite & sprite) noexcept {
		KREOGL_PROFILING_SCOPE;

		_glsl.model = sprite.transform;
		_glsl.color = sprite.color;
		_glsl.user_data = sprite.user_data;

		glBindTexture(GL_TEXTURE_2D, *sprite.texture->texture);
		shapes::draw_textured_quad();
	}
}