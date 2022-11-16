#pragma once

// stl
#include <vector>

// glm
#include <glm/glm.hpp>

// kreogl
#include "kreogl/impl/DrawParams.hpp"
#include "Uniform.hpp"

namespace kreogl {
	class KREOGL_EXPORT Shader {
	public:
		virtual ~Shader() noexcept = default;

		virtual void draw(const DrawParams & params) noexcept = 0;

	protected:
		void init(const std::string & name) noexcept; // must be called for initialization
		void addSourceFile(const char * glsl, GLenum type) noexcept;
		virtual void addSourceFiles() noexcept = 0;
		virtual std::vector<UniformBase *> getUniforms() noexcept = 0;

	protected:
		struct KREOGL_EXPORT UniformUseChecker {
#ifndef NDEBUG
			~UniformUseChecker() noexcept;
			std::vector<UniformBase *> & uniforms;
#endif
			bool shouldCheck = true;
		};
		UniformUseChecker use(bool shouldCheck = true) noexcept;
		void useWithoutUniformCheck() const noexcept;

	private:
		void link() const noexcept;
		void getUniformLocations() noexcept;

	private:
		GLuint _program = -1;
		std::vector<UniformBase *> _uniforms;
	};
}