#pragma once

// stl
#include <vector>

// glm
#include <glm/glm.hpp>

// kreogl
#include "raii/buffer.hpp"
#include "raii/frame_buffer.hpp"
#include "raii/texture.hpp"

namespace kreogl {
	class KREOGL_EXPORT gbuffer {
	public:
		enum class texture {
			position,
			normal,
			diffuse_and_should_ignore_lighting,
			specular,
			user_data,
			count
		};

	public:
		gbuffer(const glm::ivec2 & size) noexcept;

	public:
		void resize(const glm::ivec2 & size) noexcept;
		const glm::ivec2 & get_size() const noexcept { return _size; }

	public:
		void bind_for_writing() const noexcept;
		void bind_for_reading() const noexcept;

	public:
		void get_texture_content(texture texture, void * content) const noexcept;
		glm::vec4 read_pixel(texture texture, const glm::ivec2 & pixel) const noexcept;

	public:
		const kreogl::texture & get_texture(texture texture) const noexcept { return _textures[(int)texture]; }
		GLuint get_frame_buffer() const noexcept { return _frame_buffer; }

	public:
		static constexpr auto TEXTURE_COMPONENTS = 4;

	private:
		frame_buffer _frame_buffer;
		kreogl::texture _depth_texture;
		glm::ivec2 _size;

		std::vector<kreogl::texture> _textures;
	};
}