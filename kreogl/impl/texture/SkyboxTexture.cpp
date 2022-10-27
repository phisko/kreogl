#include "SkyboxTexture.hpp"

// stl
#include <cassert>

// stb
#include "stb_image.h"

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	SkyboxTexture::SkyboxTexture(const char * left, const char * right, const char * top, const char * bottom, const char * front, const char * back) noexcept {
		KREOGL_PROFILING_SCOPE;

		struct ImageFile {
			ImageFile(const char * file) noexcept {
				data = stbi_load(file, &width, &height, &nrChannels, 0);
				assert(data);
			}

			~ImageFile() noexcept {
				if (data)
					stbi_image_free(data);
			}

			int width, height, nrChannels;
			void * data = nullptr;
		};

		const ImageFile imageFiles[] = {
			{ right }, { left }, { top }, { bottom }, { front }, { back }
		};

		for (const auto & imageFile : imageFiles)
			if (!imageFile.data)
				return;

		texture = Texture{};
		glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
		for (unsigned int i = 0; i < std::extent_v<decltype(imageFiles)>; ++i) {
			const auto & imageFile = imageFiles[i];
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imageFile.width, imageFile.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageFile.data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}