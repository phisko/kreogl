#pragma once

#include <string>
#include <unordered_map>
#include "FontFamily.hpp"

namespace kreogl {
	class FontLibrary {
	public:
		FontFamily & getFontFamily(const std::string & file) noexcept;

	private:
		std::unordered_map<std::string, FontFamily> _fonts;
	};
}