#include "skybox_shader.hpp"

// kreogl
#include "kreogl/camera.hpp"
#include "kreogl/world.hpp"
#include "kreogl/impl/raii/scoped_gl_feature.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	skybox_shader::skybox_shader() noexcept {
		KREOGL_PROFILING_SCOPE;

		init("skybox_shader");

		use_without_uniform_check();
		_glsl.tex = (int)gbuffer::texture::count;
	}

	void skybox_shader::add_source_files() noexcept {
		KREOGL_PROFILING_SCOPE;

		add_source_file(skybox_glsl::vert, GL_VERTEX_SHADER);
		add_source_file(skybox_glsl::frag, GL_FRAGMENT_SHADER);
	}

	std::vector<uniform_base *> skybox_shader::get_uniforms() noexcept {
		KREOGL_PROFILING_SCOPE;

		return {
			&_glsl.proj,
			&_glsl.view,
			&_glsl.tex,
			&_glsl.color
		};
	}

	void skybox_shader::draw(const draw_params & params) noexcept {
		KREOGL_PROFILING_SCOPE;

		if (!params.world.skybox.texture || !params.world.skybox.texture->texture)
			return;

		const auto uniform_checker = use();

		GLint fbo;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
		assert(fbo != 0);

		const scoped_gl_feature blend(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		const scoped_gl_feature depth(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		_glsl.view = glm::mat4(glm::mat3(params.camera.get_view_matrix()));
		_glsl.proj = params.camera.get_proj_matrix();

		glActiveTexture((GLenum)(GL_TEXTURE0 + (int)gbuffer::texture::count));

		_glsl.color = params.world.skybox.color;

		glBindTexture(GL_TEXTURE_CUBE_MAP, *params.world.skybox.texture->texture);
		draw_skybox();

		glDepthFunc(GL_LESS);
	}

	void skybox_shader::draw_skybox() noexcept {
		KREOGL_PROFILING_SCOPE;

		static GLuint vao = 0;
		static GLuint vbo;
		if (vao == 0) {
			const float vertices[] = {
				// positions
				-1.0f, 1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, 1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,

				-1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,
				-1.0f, 1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f,

				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f,

				-1.0f, 1.0f, -1.0f,
				1.0f, 1.0f, -1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, 1.0f
			};

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		}

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}