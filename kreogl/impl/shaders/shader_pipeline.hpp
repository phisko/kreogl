#pragma once

// stl
#include <optional>
#include <unordered_map>

// kreogl
#include "shader.hpp"
#include "shader_step.hpp"

namespace kreogl {
	class KREOGL_EXPORT shader_pipeline {
#ifdef KREOGL_DEFAULT_SHADERS
	public:
		static const shader_pipeline & get_default_shaders() noexcept;
#endif

	public:
		void add_shader(shader_step step, shader & shader) noexcept;
		void remove_shader(shader_step step, const shader & shader) noexcept;
		void run_shaders(shader_step step, const draw_params & params) const noexcept;
		const std::vector<shader *> * get_shaders(shader_step step) const noexcept;

	private:
		std::unordered_map<shader_step, std::vector<shader *>> _shaders_per_step;
	};
}