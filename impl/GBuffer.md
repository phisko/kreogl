# [GBuffer](GBuffer.hpp)

The GBuffer for a [viewport](Viewport.md).

## Members

### Texture

```cpp
enum class Texture {
	Position,
	Normal,
	Diffuse,
	Specular,
	UserData,
	Count
};
```

One entry for each texture in the buffer. The `UserData` texture is used to store arbitrary data, which may be used by shaders or mapped to the CPU later.

### resize

```cpp
void resize(const glm::ivec2 & size) noexcept;
```

Changes the size of the underlying textures.

### bindForWriting

```cpp
void bindForWriting() const noexcept;
```

Binds the underlying textures as output channels for shaders.

### bindForReading

```cpp
void bindForReading() const noexcept;
```

Binds the underlying textures to the GL_TEXTURE0, GL_TEXTURE1, etc... textures, so that shaders may read from them.

### getMappedTexture

```cpp
struct MappedTexture {
	const float * getData() const noexcept;
};

MappedTexture getMappedTexture(size_t textureIndex) noexcept;
```

Returns an RAII object that contains a texture mapped to CPU memory.

### getTexture

```cpp
const kreogl::Texture & getTexture(Texture texture) const noexcept;
```

### getFrameBuffer

```cpp
GLuint getFrameBuffer() const noexcept;
```