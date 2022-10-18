#include "ImageTexture.hpp"

// stl
#include <cassert>

// stb
#include <stb_image.h>

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    namespace {
        static void loadTexture(const void * data, ImageTexture & texture) noexcept {
            KREOGL_PROFILING_SCOPE;

            texture.texture = kreogl::Texture{};

            GLenum format;
            switch (texture.components) {
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
                    return;
            }

            glBindTexture(GL_TEXTURE_2D, *texture.texture);
            glTexImage2D(GL_TEXTURE_2D, 0, (GLint)format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }

    ImageTexture::ImageTexture(const char * file) noexcept {
        KREOGL_PROFILING_SCOPE;

        const auto data = stbi_load(file, &width, &height, &components, 0);
        if (data == nullptr) {
            assert(false);
            return;
        }

        loadTexture(data, *this);
        stbi_image_free(data);
    }

    ImageTexture::ImageTexture(const void * data, unsigned int width, unsigned int height) noexcept {
        KREOGL_PROFILING_SCOPE;

        static constexpr auto expectedChannels = 4;

        const bool mustLoadFromMemory = height == 0;

        if (mustLoadFromMemory) {
            data = stbi_load_from_memory((const unsigned char *)data, (int)width, &this->width, &this->height, &this->components, expectedChannels);
            if (data == nullptr) {
                assert(false);
                return;
            }
        }
        else {
            this->width = (int)width;
            this->height = (int)height;
            this->components = expectedChannels;
        }

        loadTexture(data, *this);

        if (mustLoadFromMemory)
            stbi_image_free(const_cast<void *>(data));
    }

    bool ImageTexture::isSupportedFormat(const char * file) noexcept {
        KREOGL_PROFILING_SCOPE;

        int x, y, comp;
        return stbi_info(file, &x, &y, &comp);
    }
}