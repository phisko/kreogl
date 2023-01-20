# [texture_data](texture_data.hpp)

Holds the data required to load a [texture](../raii/texture.md).

## Members

### Attributes

```cpp
void * data = nullptr;
int width = 0;
int height = 0;
int components = 0;
```

The loaded texture data.

### Constructors

```cpp
texture_data(const char * file) noexcept;
```

Loads the data from `file`.

```cpp
texture_data(const void * data, unsigned int width, unsigned int height) noexcept;
```

Loads the data from memory (uncompressing it if necessary).

### load_to_texture

```cpp
texture load_to_texture() const noexcept;
```

Loads the data into an OpenGL [texture](../raii/texture.md).

### is_supported_format

```cpp
static bool is_supported_format(const char * file) noexcept;
```

Returns whether `file` is a supported image type.