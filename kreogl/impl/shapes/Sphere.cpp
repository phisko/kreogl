#include "Quad.hpp"

#include <cmath>
#include <vector>
#include <GL/glew.h>
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl::shapes {
	namespace {
#define X .525731112119133606f
#define Z .850650808352039932f
		static constexpr GLfloat vdata[12][3] = {
			{ -X, 0.f, Z }, { X, 0.f, Z }, { -X, 0.f, -Z }, { X, 0.f, -Z },
			{ 0.f, Z, X }, { 0.f, Z, -X }, { 0.f, -Z, X }, { 0.f, -Z, -X },
			{ Z, X, 0.f }, { -Z, X, 0.f }, { Z, -X, 0.f }, { -Z, -X, 0.f }
		};

		static constexpr GLuint tindices[20][3] = {
			{ 0, 4, 1 }, { 0, 9, 4 }, { 9, 5, 4 }, { 4, 5, 8 }, { 4, 8, 1 },
			{ 8, 10, 1 }, { 8, 3, 10 }, { 5, 3, 8 }, { 5, 2, 3 }, { 2, 7, 3 },
			{ 7, 10, 3 }, { 7, 6, 10 }, { 7, 11, 6 }, { 11, 0, 6 }, { 0, 1, 6 },
			{ 6, 1, 10 }, { 9, 0, 11 }, { 9, 11, 2 }, { 9, 2, 5 }, { 7, 2, 11 }
		};

		static void normalize(GLfloat * a) noexcept {
			GLfloat d = std::sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
			a[0] /= d;
			a[1] /= d;
			a[2] /= d;
		}

		static void drawtri(std::vector<float> & ret, const GLfloat * a, const GLfloat * b, const GLfloat * c, int div, float r) noexcept {
			if (div <= 0) {
				ret.push_back(a[0] * r);
				ret.push_back(a[1] * r);
				ret.push_back(a[2] * r);
				ret.push_back(b[0] * r);
				ret.push_back(b[1] * r);
				ret.push_back(b[2] * r);
				ret.push_back(c[0] * r);
				ret.push_back(c[1] * r);
				ret.push_back(c[2] * r);
			} else {
				GLfloat ab[3], ac[3], bc[3];
				for (int i = 0; i < 3; i++) {
					ab[i] = (a[i] + b[i]) / 2;
					ac[i] = (a[i] + c[i]) / 2;
					bc[i] = (b[i] + c[i]) / 2;
				}
				normalize(ab);
				normalize(ac);
				normalize(bc);
				drawtri(ret, a, ab, ac, div - 1, r);
				drawtri(ret, b, bc, ab, div - 1, r);
				drawtri(ret, c, ac, bc, div - 1, r);
				drawtri(ret, ab, bc, ac, div - 1, r); //<--Comment this line and sphere looks really cool!
			}
		}
	}

	void drawSphere() noexcept {
		KREOGL_PROFILING_SCOPE;

		static constexpr auto ndiv = 5;
		static const auto radius = .5f;

		static GLuint vao = -1;
		static size_t count = 0;

		if (vao == -1) {
			std::vector<float> vertices;

			for (const auto & tindex : tindices)
				drawtri(vertices, vdata[tindex[0]], vdata[tindex[1]], vdata[tindex[2]], ndiv, radius);

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(vertices.size() * sizeof(float)), vertices.data(), GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

			count = vertices.size();
		}

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)count);
		glBindVertexArray(0);
	}
}