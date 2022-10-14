# [FontLibrary](FontLibrary.hpp)

Holds the [FontFamilies](FontFamily.md) for all fonts used by the [TextShader](TextShader.md).

## Members

### getFontFamily

```cpp
FontFamily & getFontFamily(const std::string & file) noexcept;
```

Returns the `FontFamily` for `file`, creating it if needed.