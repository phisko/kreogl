#include "Box.hpp"

// gl
#include <GL/glew.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl::shapes {
	void drawBox() noexcept {
		KREOGL_PROFILING_SCOPE;

		static GLuint vao = -1;
		if (vao == -1) {
			static constexpr float vertices[] = {
				// positions
				-.5f, .5f, -.5f,
				-.5f, -.5f, -.5f,
				.5f, -.5f, -.5f,
				.5f, -.5f, -.5f,
				.5f, .5f, -.5f,
				-.5f, .5f, -.5f,

				-.5f, -.5f, .5f,
				-.5f, -.5f, -.5f,
				-.5f, .5f, -.5f,
				-.5f, .5f, -.5f,
				-.5f, .5f, .5f,
				-.5f, -.5f, .5f,

				.5f, -.5f, -.5f,
				.5f, -.5f, .5f,
				.5f, .5f, .5f,
				.5f, .5f, .5f,
				.5f, .5f, -.5f,
				.5f, -.5f, -.5f,

				-.5f, -.5f, .5f,
				-.5f, .5f, .5f,
				.5f, .5f, .5f,
				.5f, .5f, .5f,
				.5f, -.5f, .5f,
				-.5f, -.5f, .5f,

				-.5f, .5f, -.5f,
				.5f, .5f, -.5f,
				.5f, .5f, .5f,
				.5f, .5f, .5f,
				-.5f, .5f, .5f,
				-.5f, .5f, -.5f,

				-.5f, -.5f, -.5f,
				-.5f, -.5f, .5f,
				.5f, -.5f, -.5f,
				.5f, -.5f, -.5f,
				-.5f, -.5f, .5f,
				.5f, -.5f, .5f
			};

			glGenVertexArrays(1, &vao);
			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		}

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}