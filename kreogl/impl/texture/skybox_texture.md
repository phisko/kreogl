# [skybox_texture](skybox_texture.hpp)

Helper class to load a set of files into a skybox [texture](../impl/raii/texture.md).

## Members

### Constructor

```cpp
skybox_texture(
	const char * left, const char * right,
	const char * top, const char * bottom,
	const char * front, const char * back) noexcept;
```

Attemps to load the specified files into a texture.

### texture

```cpp
std::optional<texture> texture;
```

The underlying texture.