#pragma once

// stl
#include <optional>
#include <unordered_map>

// kreogl
#include "Shader.hpp"
#include "ShaderStep.hpp"

namespace kreogl {
	class ShaderPipeline {
#ifdef KREOGL_DEFAULT_SHADERS
	public:
		static const ShaderPipeline & getDefaultShaders() noexcept;
#endif

	public:
		void addShader(ShaderStep step, Shader & shader) noexcept;
		void removeShader(ShaderStep step, const Shader & shader) noexcept;
		void runShaders(ShaderStep step, const DrawParams & params) const noexcept;
		const std::vector<Shader *> * getShaders(ShaderStep step) const noexcept;

	private:
		std::unordered_map<ShaderStep, std::vector<Shader *>> _shadersPerStep;
	};
}