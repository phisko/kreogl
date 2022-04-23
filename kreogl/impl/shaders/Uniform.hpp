#pragma once

#include <type_traits>
#include <string>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace kreogl {
	inline void setUniform(GLint location, bool val) noexcept { glUniform1i(location, val); }

	inline void setUniform(GLint location, int val) noexcept { glUniform1i(location, val); }
	inline void setUniform(GLint location, GLuint val) noexcept { glUniform1i(location, val); }
    template<typename = std::enable_if_t<!std::is_same_v<size_t, GLuint>>>
	inline void setUniform(GLint location, size_t val) noexcept { glUniform1i(location, (GLint)val); }

	inline void setUniform(GLint location, float val) noexcept { glUniform1f(location, val); }

	inline void setUniform(GLint location, const float(&val)[3]) noexcept { glUniform3f(location, val[0], val[1], val[2]); }
	inline void setUniform(GLint location, const glm::vec3 & val) noexcept { glUniform3f(location, val.x, val.y, val.z); }

	inline void setUniform(GLint location, const float(&val)[4]) noexcept { glUniform4f(location, val[0], val[1], val[2], val[3]); }
    inline void setUniform(GLint location, const glm::vec4 & val) noexcept { glUniform4f(location, val.r, val.g, val.b, val.a); }

	inline void setUniform(GLint location, const float(&val)[2]) noexcept { glUniform2f(location, val[0], val[1]); }
	inline void setUniform(GLint location, const glm::vec2 & val) noexcept { glUniform2f(location, val.x, val.y); }

	inline void setUniform(GLint location, const glm::mat4 & val) noexcept { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val)); }

    struct UniformBase {
        std::string name;
        GLint location = -1;
    };

	template<typename T>
	struct Uniform : UniformBase {
		void operator=(const T & val) const noexcept {
			setUniform(location, val);
		}
	};
}