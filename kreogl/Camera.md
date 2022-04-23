# [Camera](Camera.hpp)

Represents an in-game camera. Contains the corresponding [viewport](impl/Viewport.md).

## Members

### position

```cpp
const glm::vec3 & getPosition() const noexcept;
void setPosition(const glm::vec3 & position) noexcept;
```

The position of the camera in the 3D world.

### direction

```cpp
const glm::vec3 & getDirection() const noexcept;
void setDirection(const glm::vec3 & direction) noexcept;
```

The direction the camera is looking.

### FOV

```cpp
float getFOV() const noexcept;
void setFOV(float fov) noexcept;
```

The camera's vertical field of view.

### viewPort

```cpp
const Viewport & getViewport() const noexcept;
Viewport & getViewportWritable() noexcept;
```

The on-screen viewport in which the camera is rendered.

### nearPlane, farPlane

```cpp
float getNearPlane() const noexcept;
void setNearPlane(float nearPlane) noexcept;

float getFarPlane() const noexcept;
void setFarPlane(float farPlane) noexcept;
```

The camera's rendering distance limits.

### viewMatrix, projMatrix

```cpp
const glm::mat4 & getViewMatrix() const noexcept;
const glm::mat4 & getProjMatrix() const noexcept;
```

The camera's view and projection matrices, calculated based on its position, direction and FOV.