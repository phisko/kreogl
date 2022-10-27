#include "FontFamily.hpp"
#include "kreogl/impl/kreogl_profiling.hpp"

namespace kreogl {
	FontFamily::FontFamily(const std::string & file) noexcept
	: _file(file)
	{
	}

	Font * FontFamily::getFont(size_t size) noexcept {
		KREOGL_PROFILING_SCOPE;

		if (const auto it = _fontsBySize.find(size); it != _fontsBySize.end())
			return &it->second;

		const auto result = _fontsBySize.emplace(size, Font{ _file.c_str(), size });
		return &result.first->second;
	}
}