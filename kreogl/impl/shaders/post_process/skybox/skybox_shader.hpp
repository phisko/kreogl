#pragma once

// kreogl
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"
#include "skybox_glsl.hpp"

namespace kreogl {
	class KREOGL_EXPORT skybox_shader : public shader, public singleton<skybox_shader> {
	public:
		skybox_shader() noexcept;

	private:
		void draw(const draw_params & params) noexcept override;
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;

	private:
		void draw_skybox() noexcept;

	private:
		skybox_glsl _glsl;
	};
}