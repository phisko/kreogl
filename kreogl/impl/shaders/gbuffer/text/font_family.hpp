#pragma once

// stl
#include <string>
#include <unordered_map>

// kreogl
#include "font.hpp"
#include "kreogl/impl/default_constructors.hpp"

namespace kreogl {
	class KREOGL_EXPORT font_family {
	public:
		KREOGL_MOVE_ONLY(font_family);

		font_family(const std::string & file) noexcept;
		font * get_font(size_t size) noexcept;

	private:
		std::unordered_map<size_t, font> _fonts_by_size;
		std::string _file;
	};
}