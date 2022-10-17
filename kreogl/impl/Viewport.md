# [Viewport](Viewport.hpp)

Represents an on-screen viewport.

## Members

### ConstructionParams

```cpp
struct ConstructionParams {
	glm::ivec2 resolution = { 0, 0 };
	glm::vec2 onScreenPosition = { 0, 0 };
	glm::vec2 onScreenSize = { 1, 1 };
	float zOrder = 1.f;
};
```

`onScreenPosition` and `onScreenSize` are screen-percentages.

### draw

```cpp
void draw(const DrawParams & params) const noexcept;
```

Draws all shaders into this viewport.

### resolution

```cpp
const glm::ivec2 & getResolution() const noexcept;
void setResolution(const glm::ivec2 & resolution) noexcept;
```

The underlying framebuffer's resolution.

### aspectRatio

```cpp
float getAspectRatio() const noexcept;
```

Returns the underlying framebuffer's aspect ratio.

### onScreenPosition, onScreenSize

```cpp
const glm::vec2 & getOnScreenPosition() const noexcept;
void setOnScreenPosition(const glm::vec2 & onScreenPosition) noexcept;

const glm::vec2 & getOnScreenSize() const noexcept;
void setOnScreenSize(const glm::vec2 & onScreenSize) noexcept;
```

The on-screen position and size for this viewport, in screen-percentages.

### zOrder

```cpp
float getZOrder() const noexcept;
void setZOrder(float zOrder) noexcept;
```

The Z order for this viewport, defining whether it will be drawn over or under others.

### getFrameBuffer, getRenderTexture

```cpp
GLuint getFrameBuffer() const noexcept;
GLuint getRenderTexture() const noexcept;
```

Return the underlying resources.

### getGBuffer

```cpp
const GBuffer & getGBuffer() const noexcept;
```

Returns the underlying [GBuffer](GBuffer.md).