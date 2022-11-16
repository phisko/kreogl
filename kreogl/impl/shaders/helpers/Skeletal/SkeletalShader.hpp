#pragma once

// kreogl
#include "SkeletalGLSL.hpp"
#include "kreogl/impl/shaders/Shader.hpp"
#include "kreogl/animation/AnimatedModel.hpp"
#include "kreogl/Object.hpp"

namespace kreogl {
	class KREOGL_EXPORT SkeletalShader {
	public:
		void uploadDefaultBones() noexcept;
		void drawObject(const Object & object) noexcept;
		virtual void setMeshUniforms(const AnimatedModel & model, unsigned int meshIndex) noexcept {}

	protected:
		SkeletalGLSL _glsl;
	};
}