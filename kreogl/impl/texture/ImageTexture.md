# [ImageTexture](ImageTexture.hpp)

Helper class to load an image file into a [Texture](../impl/RAII/Texture.md).

## Members

### Constructor

```cpp
ImageTexture(const char * file) noexcept;
```

Attempts to load `file` into a texture.

```cpp
ImageTexture(const void * data, unsigned int width, unsigned int height) noexcept;
```

Attempts to load in-memory `data` into a texture.

### isSupportedFormat

```cpp
static bool isSupportedFormat(const char * file) noexcept;
```

Returns whether `file` is a supported image type.

### width, height, components

```cpp
int width = 0;
int height = 0;
int components = 0;
```

The image's attributes.

### texture

```cpp
std::optional<Texture> texture;
```

The underlying texture.