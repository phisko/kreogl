#pragma once

// stl
#include <string>
#include <unordered_map>

// kreogl
#include "FontFamily.hpp"
#include "kreogl/impl/DefaultConstructors.hpp"

namespace kreogl {
	class KREOGL_EXPORT FontLibrary {
	public:
		KREOGL_MOVE_ONLY(FontLibrary);

		FontFamily & getFontFamily(const std::string & file) noexcept;

	private:
		std::unordered_map<std::string, FontFamily> _fonts;
	};
}