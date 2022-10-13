#include "Line.hpp"

#include <GL/glew.h>

namespace kreogl::shapes {
    void drawLine(const glm::vec3 & start, const glm::vec3 & end) noexcept {
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