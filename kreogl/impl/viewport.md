# [viewport](viewport.hpp)

Represents an on-screen viewport.

## Members

### construction_params

```cpp
struct construction_params {
	glm::ivec2 resolution = { 0, 0 };
	glm::vec2 on_screen_position = { 0, 0 };
	glm::vec2 on_screen_size = { 1, 1 };
	float z_order = 1.f;
};
```

`on_screen_position` and `on_screen_size` are screen-percentages.

### draw

```cpp
void draw(const draw_params & params) const noexcept;
```

Draws all shaders into this viewport.

### resolution

```cpp
const glm::ivec2 & get_resolution() const noexcept;
void set_resolution(const glm::ivec2 & resolution) noexcept;
```

The underlying framebuffer's resolution.

### aspectRatio

```cpp
float get_aspect_ratio() const noexcept;
```

Returns the underlying framebuffer's aspect ratio.

### on_screen_position, on_screen_size

```cpp
const glm::vec2 & get_on_screen_position() const noexcept;
void set_on_screen_position(const glm::vec2 & on_screen_position) noexcept;

const glm::vec2 & get_on_screen_size() const noexcept;
void set_on_screen_size(const glm::vec2 & on_screen_size) noexcept;
```

The on-screen position and size for this viewport, in screen-percentages.

### z_order

```cpp
float get_z_order() const noexcept;
void set_z_order(float z_order) noexcept;
```

The Z order for this viewport, defining whether it will be drawn over or under others.

### get_frame_buffer, get_render_texture

```cpp
GLuint get_frame_buffer() const noexcept;
GLuint get_render_texture() const noexcept;
```

Return the underlying resources.

### get_gbuffer

```cpp
const gbuffer & get_gbuffer() const noexcept;
```

Returns the underlying [gbuffer](gbuffer.md).