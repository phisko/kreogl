#pragma once

// stl
#include <string>
#include <unordered_map>

// kreogl
#include "Font.hpp"
#include "kreogl/impl/DefaultConstructors.hpp"

namespace kreogl {
	class KREOGL_EXPORT FontFamily {
	public:
		KREOGL_MOVE_ONLY(FontFamily);

		FontFamily(const std::string & file) noexcept;
		Font * getFont(size_t size) noexcept;

	private:
		std::unordered_map<size_t, Font> _fontsBySize;
		std::string _file;
	};
}