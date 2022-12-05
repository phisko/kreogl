#pragma once

// stl
#include <string>
#include <unordered_map>

// freetype
#include <freetype/freetype.h>

// glm
#include <glm/glm.hpp>

// kreogl
#include "kreogl/impl/default_constructors.hpp"
#include "kreogl/impl/raii/texture.hpp"

namespace kreogl {
	class KREOGL_EXPORT font {
	public:
		KREOGL_MOVE_ONLY(font);

		font(const char * file, size_t size) noexcept;

		struct KREOGL_EXPORT character {
			KREOGL_MOVE_ONLY(character);

			kreogl::texture texture;
			glm::ivec2 size;
			glm::ivec2 bearing;
			GLuint advance;
		};

		const character * get_character(unsigned long c) noexcept;
		glm::vec2 get_size_and_generate_characters(const std::string & text, float scale_x, float scale_y) noexcept;

	private:
		FT_Face face = nullptr;
		std::unordered_map<unsigned long, character> characters;

		struct free_type_library {
			~free_type_library() noexcept;
			FT_Library library;
		};

		static free_type_library g_free_type_library;
	};
}