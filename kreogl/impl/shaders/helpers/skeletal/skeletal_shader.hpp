#pragma once

// kreogl
#include "skeletal_glsl.hpp"
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/animation/animated_model.hpp"
#include "kreogl/object.hpp"

namespace kreogl {
	class KREOGL_EXPORT skeletal_shader {
	public:
		void upload_default_bones() noexcept;
		void draw_object(const object & object) noexcept;
		virtual void set_mesh_uniforms(const animated_model & model, unsigned int mesh_index) noexcept {}

	protected:
		skeletal_glsl _glsl;
	};
}