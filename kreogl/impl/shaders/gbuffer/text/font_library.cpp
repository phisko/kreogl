#include "font_library.hpp"

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	font_family & font_library::get_font_family(const std::string & file) noexcept {
		KREOGL_PROFILING_SCOPE;

		if (const auto it = _fonts.find(file); it != _fonts.end())
			return it->second;

		const auto result = _fonts.emplace(file, font_family{ file });
		return result.first->second;
	}
}