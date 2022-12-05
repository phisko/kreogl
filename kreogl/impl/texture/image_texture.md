# [image_texture](image_texture.hpp)

Helper class to load an image file into a [texture](../impl/raii/texture.md).

## Members

### Constructor

```cpp
image_texture(const char * file) noexcept;
```

Attempts to load `file` into a texture.

```cpp
image_texture(const void * data, unsigned int width, unsigned int height) noexcept;
```

Attempts to load in-memory `data` into a texture.

### is_supported_format

```cpp
static bool is_supported_format(const char * file) noexcept;
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
std::optional<texture> texture;
```

The underlying texture.