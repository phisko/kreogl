#pragma once

#include <string>
#include <unordered_map>
#include "Font.hpp"

namespace kreogl {
	class FontFamily {
	public:
		FontFamily(const std::string & file) noexcept;
		Font * getFont(size_t size) noexcept;

	private:
		std::unordered_map<size_t, Font> _fontsBySize;
		std::string _file;
	};
}