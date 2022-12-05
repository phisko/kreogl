#pragma once

// stl
#include <string>
#include <unordered_map>

// kreogl
#include "font_family.hpp"
#include "kreogl/impl/default_constructors.hpp"

namespace kreogl {
	class KREOGL_EXPORT font_library {
	public:
		KREOGL_MOVE_ONLY(font_library);

		font_family & get_font_family(const std::string & file) noexcept;

	private:
		std::unordered_map<std::string, font_family> _fonts;
	};
}