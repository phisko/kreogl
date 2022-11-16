#pragma once

// stl
#include <string>
#include <unordered_map>

// kreogl
#include "Font.hpp"

namespace kreogl {
	class KREOGL_EXPORT FontFamily {
	public:
		FontFamily(const std::string & file) noexcept;
		Font * getFont(size_t size) noexcept;

	private:
		std::unordered_map<size_t, Font> _fontsBySize;
		std::string _file;
	};
}