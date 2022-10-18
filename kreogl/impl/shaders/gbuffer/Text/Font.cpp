#include "Font.hpp"

#include <iostream>
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
    Font::FreeTypeLibrary Font::g_freetypeLibrary;

    Font::FreeTypeLibrary::~FreeTypeLibrary() noexcept {
        KREOGL_PROFILING_SCOPE;

        if (library)
            FT_Done_FreeType(library);
    }

    Font::Font(const char * file, size_t size) noexcept {
        KREOGL_PROFILING_SCOPE;

        if (!g_freetypeLibrary.library)
            if (FT_Init_FreeType(&g_freetypeLibrary.library)) {
                assert(false);
                return;
            }

        if (FT_New_Face(g_freetypeLibrary.library, file, 0, &face)) {
            std::cerr << "[FreeType] Error loading font `" << file << '`' << std::endl;
            assert(false);
            return;
        }

        if (FT_Set_Pixel_Sizes(face, 0, (FT_UInt)size)) {
            std::cerr << "[FreeType] Error setting size `" << size << "` for font `" << file << '`' << std::endl;
            assert(false);
            return;
        }
    }

    const Font::Character * Font::getCharacter(unsigned long c) noexcept {
        KREOGL_PROFILING_SCOPE;

        if (const auto it = characters.find(c); it != characters.end())
            return &it->second;

        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            assert(false);
            return nullptr;
        }

        auto & character = characters[c];
        character.size = { face->glyph->bitmap.width, face->glyph->bitmap.rows };
        character.bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top };
        character.advance = (GLuint)face->glyph->advance.x;

        glBindTexture(GL_TEXTURE_2D, character.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (GLsizei)face->glyph->bitmap.width, (GLsizei)face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return &character;
    }

    glm::vec2 Font::getSizeAndGenerateCharacters(const std::string & text, float scaleX, float scaleY) noexcept {
        KREOGL_PROFILING_SCOPE;

        glm::vec2 size{ 0.f, 0.f };

        for (const auto c : text) {
            const auto character = getCharacter(c);
            if (!character) {
                assert(false);
                continue;
            }

            size.x += (float)(character->advance >> 6) * scaleX;

            if ((float)character->size.y > size.y)
                size.y = (float)character->size.y;
        }

        size.y *= scaleY;
        return size;
    }
}