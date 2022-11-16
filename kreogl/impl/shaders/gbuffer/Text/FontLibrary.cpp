#include "FontLibrary.hpp"

// kreogl
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	FontFamily & FontLibrary::getFontFamily(const std::string & file) noexcept {
		KREOGL_PROFILING_SCOPE;

		if (const auto it = _fonts.find(file); it != _fonts.end())
			return it->second;

		const auto result = _fonts.emplace(file, FontFamily{ file });
		return result.first->second;
	}
}