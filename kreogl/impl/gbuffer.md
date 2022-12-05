# [gbuffer](gbuffer.hpp)

The gbuffer for a [viewport](viewport.md).

## Members

### texture

```cpp
enum class texture {
	position,
	normal,
	diffuse_and_should_ignore_lighting,
	specular,
	user_data,
	count
};
```

One entry for each texture in the buffer. The `user_data` texture is used to store arbitrary data, which may be used by shaders or mapped to the CPU later.

### resize

```cpp
void resize(const glm::ivec2 & size) noexcept;
```

Changes the size of the underlying textures.

### bind_for_writing

```cpp
void bind_for_writing() const noexcept;
```

Binds the underlying textures as output channels for shaders.

### bind_for_reading

```cpp
void bind_for_reading() const noexcept;
```

Binds the underlying textures to the GL_TEXTURE0, GL_TEXTURE1, etc... textures, so that shaders may read from them.

### get_texture_content

```cpp
void get_texture_content(texture texture, void * content) const noexcept;
```

Returns the content of one of the underlying textures.

### read_pixel

```cpp
glm::vec4 read_pixel(texture texture, const glm::ivec2 & pixel) const noexcept;
```

Returns the pixel value from one of the underlying textures.

### get_texture

```cpp
const kreogl::texture & get_texture(texture texture) const noexcept;
```

### get_frame_buffer

```cpp
GLuint get_frame_buffer() const noexcept;
```