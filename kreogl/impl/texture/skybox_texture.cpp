#include "skybox_texture.hpp"

// stl
#include <cassert>

// stb
#include <stb_image.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	skybox_texture::skybox_texture(const texture_data & left, const texture_data & right, const texture_data & top, const texture_data & bottom, const texture_data & front, const texture_data & back) noexcept {
		KREOGL_PROFILING_SCOPE;

		const texture_data * texture_datas[] = {
			&right, &left,
			&top, &bottom,
			&front, &back
		};

		texture = kreogl::texture{};
		glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
		for (unsigned int i = 0; i < std::extent_v<decltype(texture_datas)>; ++i) {
			const auto data = texture_datas[i];
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, data->width, data->height, 0, GL_RGB, GL_UNSIGNED_BYTE, data->data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}