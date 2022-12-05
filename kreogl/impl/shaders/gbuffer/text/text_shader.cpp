#include "text_shader.hpp"

// kreogl
#include "kreogl/impl/shaders/helpers/apply_transparency/apply_transparency_glsl.hpp"
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "font_library.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	text_shader::text_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("text_shader");

		use_without_uniform_check();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		_glsl.tex = (int)gbuffer::texture::count;

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	}

	void text_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(text_glsl::vert, GL_VERTEX_SHADER);
		add_source_file(text_glsl::frag, GL_FRAGMENT_SHADER);
		add_source_file(apply_transparency_glsl::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<uniform_base *> text_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			&_glsl.model,
			&_glsl.view,
			&_glsl.proj,
			&_glsl.view_pos,
			&_glsl.tex,
			&_glsl.color,
			&_glsl.user_data
		};
	}

	void text_shader::draw(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto uniform_checker = use(false);

		_glsl.view_pos = params.camera.get_position();

		glActiveTexture((GLenum)(GL_TEXTURE0 + (int)gbuffer::texture::count));
		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		_glsl.view = glm::mat4{ 1.f };
		_glsl.proj = glm::mat4{ 1.f };
		for (const auto text_2d : params.world.get_text_2ds()) {
			uniform_checker.should_check = true;
			draw(params, *text_2d);
		}

		_glsl.view = params.camera.get_view_matrix();
		_glsl.proj = params.camera.get_proj_matrix();
		for (const auto text_3d : params.world.get_text_3ds()) {
			uniform_checker.should_check = true;
			draw(params, *text_3d);
		}
	}

	void text_shader::draw(const draw_params & params, const text & text) noexcept {
		KREOGL_PROFILING_SCOPE;

		auto & font_family = _fonts.get_font_family(text.font);
		const auto font = font_family.get_font(text.font_size);
		if (!font) {
			assert(false);
			return;
		}

		_glsl.model = text.transform;
		_glsl.color = text.color;
		_glsl.user_data = text.user_data;

		const auto scale_y = 1.f / (float)text.font_size;
		const auto scale_x = scale_y / params.camera.get_viewport().get_aspect_ratio();

		// Calculate text starting position
		const auto size = font->get_size_and_generate_characters(text.value, scale_x, scale_y);
		const auto y = -size.y / 2.f;
		auto x = 0.f;
		switch (text.alignment) {
			case text::alignment_type::left:
				break;
			case text::alignment_type::center:
				x = -size.x / 2.f;
				break;
			case text::alignment_type::right:
				x = -size.x;
				break;
			default:
				assert(false); // non-exhaustive switch
				break;
		}

		for (const auto c : text.value) {
			const auto character = font->get_character(c);
			if (!character) {
				assert(false);
				continue;
			}

			glBindTexture(GL_TEXTURE_2D, character->texture);

			const auto xpos = x + (float)character->bearing.x * scale_x;
			const auto ypos = y - (float)(character->size.y - character->bearing.y) * scale_y;

			const auto w = (float)character->size.x * scale_x;
			const auto h = (float)character->size.y * scale_y;

			const float vertices[6][4] = {
				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos, ypos, 0.0, 1.0 },
				{ xpos + w, ypos, 1.0, 1.0 },

				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos + w, ypos, 1.0, 1.0 },
				{ xpos + w, ypos + h, 1.0, 0.0 }
			};

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (float)(character->advance >> 6) * scale_x;
		}
	}
}