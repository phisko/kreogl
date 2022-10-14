#include "FontLibrary.hpp"

namespace kreogl {
    FontFamily & FontLibrary::getFontFamily(const std::string & file) noexcept {
        if (const auto it = _fonts.find(file); it != _fonts.end())
            return it->second;

        const auto result = _fonts.emplace(file, FontFamily{ file });
        return result.first->second;
    }
}