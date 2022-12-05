#include "line.hpp"

// gl
#include <GL/glew.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl::shapes {
	void draw_line(const glm::vec3 & start, const glm::vec3 & end) noexcept {
		KREOGL_PROFILING_SCOPE;

		static GLuint vao = -1;
		static GLuint vbo;

		if (vao == -1) {
			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		}

		glBindVertexArray(vao);

		const float vertices[] = {
			start.x, start.y, start.z,
			end.x, end.y, end.z
		};
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_DYNAMIC_DRAW);

		glDrawArrays(GL_LINES, 0, 2);
	}
}