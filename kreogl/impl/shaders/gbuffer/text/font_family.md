# [font_family](font_family.hpp)

Holds all the [fonts](font.md) for a given file.

## Members

### Ctor

```cpp
font_family(const std::string & file) noexcept;
```

### get_font

```cpp
font * get_font(size_t size) noexcept;
```

Returns the `font` for `size`, generating it if required.
