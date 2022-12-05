# [font_library](font_library.hpp)

Holds the [font_families](font_family.md) for all fonts used by the [text_shader](text_shader.md).

## Members

### get_font_family

```cpp
font_family & get_font_family(const std::string & file) noexcept;
```

Returns the `font_family` for `file`, creating it if needed.