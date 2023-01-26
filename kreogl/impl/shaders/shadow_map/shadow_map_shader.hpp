#pragma once

// stl
#include <concepts>

// kreogl
#include "kreogl/impl/shaders/shader.hpp"
#include "kreogl/lights/directional_light.hpp"
#include "kreogl/lights/spot_light.hpp"
#include "kreogl/impl/draw_params.hpp"

namespace kreogl {
	class KREOGL_EXPORT shadow_map_shader : public shader {
	protected:
		virtual void draw_to_texture(GLuint texture, const glm::mat4 & light_space_matrix, const draw_params & params) noexcept = 0;

	public:
		void draw(const directional_light & light, const draw_params & params) noexcept;
		void draw(const spot_light & light, const draw_params & params) noexcept;

	private:
		void draw(const draw_params & params) noexcept final {}

		template<typename T, std::invocable Func>
		void draw_impl(T & shadow_map, Func && draw, const draw_params & params) noexcept;
	};
}