#pragma once

// stl
#include <type_traits>
#include <string>

// gl
#include <GL/glew.h>
#include <GL/gl.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace kreogl {
	inline void set_uniform(GLint location, bool val) noexcept { glUniform1i(location, val); }

	inline void set_uniform(GLint location, int val) noexcept { glUniform1i(location, val); }
	inline void set_uniform(GLint location, GLuint val) noexcept { glUniform1i(location, val); }
	template<typename = std::enable_if_t<!std::is_same_v<size_t, GLuint>>>
	inline void set_uniform(GLint location, size_t val) noexcept { glUniform1i(location, (GLint)val); }

	inline void set_uniform(GLint location, float val) noexcept { glUniform1f(location, val); }

	inline void set_uniform(GLint location, const float (&val)[3]) noexcept { glUniform3f(location, val[0], val[1], val[2]); }
	inline void set_uniform(GLint location, const glm::vec3 & val) noexcept { glUniform3f(location, val.x, val.y, val.z); }

	inline void set_uniform(GLint location, const float (&val)[4]) noexcept { glUniform4f(location, val[0], val[1], val[2], val[3]); }
	inline void set_uniform(GLint location, const glm::vec4 & val) noexcept { glUniform4f(location, val.r, val.g, val.b, val.a); }

	inline void set_uniform(GLint location, const float (&val)[2]) noexcept { glUniform2f(location, val[0], val[1]); }
	inline void set_uniform(GLint location, const glm::vec2 & val) noexcept { glUniform2f(location, val.x, val.y); }

	inline void set_uniform(GLint location, const glm::mat4 & val) noexcept { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val)); }

	struct KREOGL_EXPORT uniform_base {
		std::string name;
		GLint location = -1;
#ifndef NDEBUG
		bool used = false;
#endif

		void mark_as_used() {
#ifndef NDEBUG
			used = true;
#endif
		}
	};

	template<typename T>
	struct uniform : uniform_base {
		void operator=(const T & val) noexcept {
			mark_as_used();
			set_uniform(location, val);
		}
	};
}