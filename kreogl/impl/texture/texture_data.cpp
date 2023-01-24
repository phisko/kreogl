#include "texture_data.hpp"

// stl
#include <cassert>

// stb
#include <stb_image.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	texture_data::texture_data(const char * file) noexcept {
		KREOGL_PROFILING_SCOPE;
		data = stbi_load(file, &width, &height, &components, 0);
	}

	texture_data::texture_data(const void * data, unsigned int width, unsigned int height) noexcept {
		KREOGL_PROFILING_SCOPE;

		static constexpr auto expected_channels = 4;

		const bool must_load_from_memory = height == 0;

		if (must_load_from_memory) {
			this->data = stbi_load_from_memory((const unsigned char *)data, (int)width, &this->width, &this->height, &this->components, expected_channels);
			return;
		}

		this->data = (void *)data;
		this->width = (int)width;
		this->height = (int)height;
		this->components = expected_channels;
		this->must_release = false;
	}

	texture_data::~texture_data() noexcept {
		KREOGL_PROFILING_SCOPE;

		if (must_release)
			stbi_image_free(data);
	}

	texture_data::texture_data(texture_data && other) noexcept {
		*this = std::move(other);
	}

	texture_data & texture_data::operator=(texture_data && rhs) noexcept {
		std::swap(data, rhs.data);
		std::swap(width, rhs.width);
		std::swap(height, rhs.height);
		std::swap(components, rhs.components);
		std::swap(must_release, rhs.must_release);
		return *this;
	}

	texture texture_data::load_to_texture() const noexcept {
		KREOGL_PROFILING_SCOPE;

		texture texture;

		GLenum format;
		switch (components) {
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				assert(false); // Incompatible number of texture components
				return texture;
		}

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return texture;
	}

	bool texture_data::is_supported_format(const char * file) noexcept {
		KREOGL_PROFILING_SCOPE;

		int x, y, comp;
		return stbi_info(file, &x, &y, &comp);
	}
}