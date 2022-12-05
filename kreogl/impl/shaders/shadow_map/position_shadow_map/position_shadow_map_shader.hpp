#pragma once

// kreogl
#include "kreogl/impl/shaders/helpers/position_proj_view_model/position_proj_view_model_glsl.hpp"
#include "kreogl/impl/shaders/shadow_map/shadow_map_shader.hpp"
#include "kreogl/impl/shaders/singleton.hpp"

namespace kreogl {
	class KREOGL_EXPORT position_shadow_map_shader : public shadow_map_shader, public singleton<position_shadow_map_shader> {
	public:
		position_shadow_map_shader() noexcept;

	private:
		void add_source_files() noexcept override;
		std::vector<uniform_base *> get_uniforms() noexcept override;
		void draw_to_texture(GLuint texture, const glm::mat4 & light_space_matrix, const draw_params & params) noexcept override;

	private:
		position_proj_view_model_glsl _glsl;
	};
}