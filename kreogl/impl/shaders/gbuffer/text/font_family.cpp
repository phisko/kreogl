#include "font_family.hpp"

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	font_family::font_family(const std::string & file) noexcept
	: _file(file)
	{
	}

	font * font_family::get_font(size_t size) noexcept {
		KREOGL_PROFILING_SCOPE;

		if (const auto it = _fonts_by_size.find(size); it != _fonts_by_size.end())
			return &it->second;

		const auto result = _fonts_by_size.emplace(size, font{ _file.c_str(), size });
		return &result.first->second;
	}
}