#pragma once

// glm
#include <glm/glm.hpp>

// kreogl
#include "draw_params.hpp"
#include "gbuffer.hpp"
#include "raii/frame_buffer.hpp"
#include "raii/texture.hpp"

namespace kreogl {
	class camera;

	class KREOGL_EXPORT viewport {
	public:
		struct KREOGL_EXPORT construction_params {
			glm::ivec2 resolution = { 0, 0 };
			glm::vec2 on_screen_position = { 0, 0 };
			glm::vec2 on_screen_size = { 1, 1 };
			float z_order = 1.f;
		};
		viewport(const construction_params & params) noexcept;

		void draw(const draw_params & params) const noexcept;

	public:
		const glm::ivec2 & get_resolution() const noexcept { return _resolution; }
		void set_resolution(const glm::ivec2 & resolution) noexcept;

		float get_aspect_ratio() const noexcept { return (float)_resolution.x / (float)_resolution.y; }

		const glm::vec2 & get_on_screen_position() const noexcept { return _on_screen_position; }
		void set_on_screen_position(const glm::vec2 & on_screen_position) noexcept { _on_screen_position = on_screen_position; }

		const glm::vec2 & get_on_screen_size() const noexcept { return _on_screen_size; }
		void set_on_screen_size(const glm::vec2 & on_screen_size) noexcept { _on_screen_size = on_screen_size; }

		float get_z_order() const noexcept { return _z_order; }
		void set_z_order(float z_order) noexcept { _z_order = z_order; }

		GLuint get_frame_buffer() const noexcept { return _frame_buffer; }
		GLuint get_render_texture() const noexcept { return _render_texture; }

		gbuffer & get_gbuffer() noexcept { return _gbuffer; }
		const gbuffer & get_gbuffer() const noexcept { return _gbuffer; }

	private:
		void fill_gbuffer(const draw_params & params) const noexcept;
		void render_to_texture(const draw_params & params) const noexcept;

	private:
		glm::ivec2 _resolution;
		glm::vec2 _on_screen_position;
		glm::vec2 _on_screen_size;
		float _z_order;

		frame_buffer _frame_buffer;
		texture _render_texture;
		texture _depth_texture;
		gbuffer _gbuffer;
	};
}