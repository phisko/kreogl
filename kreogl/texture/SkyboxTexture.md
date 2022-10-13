# [SkyboxTexture](SkyboxTexture.hpp)

Helper class to load a set of files into a skybox [Texture](../impl/RAII/Texture.md).

## Members

### Constructor

```cpp
SkyboxTexture(
	const char * left, const char * right,
	const char * top, const char * bottom,
	const char * front, const char * back) noexcept;
```

Attemps to load the specified files into a texture.

### texture

```cpp
std::optional<Texture> texture;
```

The underlying texture.