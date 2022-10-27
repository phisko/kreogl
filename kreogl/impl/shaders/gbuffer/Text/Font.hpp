#pragma once

// stl
#include <string>
#include <unordered_map>

// freetype
#include <freetype/freetype.h>

// glm
#include <glm/glm.hpp>

// kreogl
#include "kreogl/impl/RAII/Texture.hpp"

namespace kreogl {
	class Font {
	public:
		Font(const char * file, size_t size) noexcept;

		struct Character {
			Texture texture;
			glm::ivec2 size;
			glm::ivec2 bearing;
			GLuint advance;
		};

		const Character * getCharacter(unsigned long c) noexcept;
		glm::vec2 getSizeAndGenerateCharacters(const std::string & text, float scaleX, float scaleY) noexcept;

	private:
		FT_Face face = nullptr;
		std::unordered_map<unsigned long, Character> characters;

		struct FreeTypeLibrary {
			~FreeTypeLibrary() noexcept;
			FT_Library library;
		};

		static FreeTypeLibrary g_freetypeLibrary;
	};
}