# [FontFamily](FontFamily.hpp)

Holds all the [Fonts](Font.md) for a given file.

## Members

### Ctor

```cpp
FontFamily(const std::string & file) noexcept;
```

### getFont

```cpp
Font * getFont(size_t size) noexcept;
```

Returns the `Font` for `size`, generating it if required.
