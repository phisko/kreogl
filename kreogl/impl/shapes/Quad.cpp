#include "Quad.hpp"

#include <GL/glew.h>
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl::shapes {
    void drawQuad() noexcept {
        KREOGL_PROFILING_SCOPE;

        static GLuint vao = 0;
        static GLuint vbo;

        if (vao == 0) {
            const float vertices[] = {
                // positions
                -1.0f,  1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f,
                1.0f,  1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        }

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
}