#include "textured_quad.hpp"

// gl
#include <GL/glew.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl::shapes {
	void draw_textured_quad() noexcept {
		KREOGL_PROFILING_SCOPE;

		static GLuint vao = 0;
		static GLuint vbo;

		if (vao == 0) {
			const float vertices[] = {
				// positions		// tex_coords
				-1.0f, -1.0f, 0.0f, 0.f, 1.f,
				-1.0f, 1.0f, 0.0f, 0.f, 0.f,
				1.0f, -1.0f, 0.0f, 1.f, 1.f,
				1.0f, 1.0f, 0.0f, 1.f, 0.f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
		}

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}