#pragma once

// stl
#include <vector>

// glm
#include <glm/glm.hpp>

// kreogl
#include "kreogl/impl/draw_params.hpp"
#include "uniform.hpp"

namespace kreogl {
	class KREOGL_EXPORT shader {
	public:
		virtual ~shader() noexcept = default;

		virtual void draw(const draw_params & params) noexcept = 0;

	protected:
		void init(const std::string & name) noexcept; // must be called for initialization
		void add_source_file(const char * glsl, GLenum type) noexcept;
		virtual void add_source_files() noexcept = 0;
		virtual std::vector<uniform_base *> get_uniforms() noexcept = 0;

	protected:
		struct KREOGL_EXPORT uniform_use_checker {
#ifndef NDEBUG
			~uniform_use_checker() noexcept;
			std::vector<uniform_base *> & uniforms;
#endif
			bool should_check = true;
		};
		uniform_use_checker use(bool should_check = true) noexcept;
		void use_without_uniform_check() const noexcept;

	private:
		void link() const noexcept;
		void get_uniform_locations() noexcept;

	private:
		GLuint _program = -1;
		std::vector<uniform_base *> _uniforms;
	};
}