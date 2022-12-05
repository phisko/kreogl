# [camera](camera.hpp)

Represents an in-game camera. Contains the corresponding [viewport](impl/viewport.md).

## Members

### position

```cpp
const glm::vec3 & get_position() const noexcept;
void set_position(const glm::vec3 & position) noexcept;
```

The position of the camera in the 3D world.

### direction

```cpp
const glm::vec3 & get_direction() const noexcept;
void set_direction(const glm::vec3 & direction) noexcept;
```

The direction the camera is looking.

### FOV

```cpp
float get_fov() const noexcept;
void set_fov(float fov) noexcept;
```

The camera's vertical field of view.

### viewPort

```cpp
const viewport & get_viewport() const noexcept;
viewport & get_viewport_writable() noexcept;
```

The on-screen viewport in which the camera is rendered.

### near_plane, far_plane

```cpp
float get_near_plane() const noexcept;
void set_near_plane(float near_plane) noexcept;

float get_far_plane() const noexcept;
void set_far_plane(float far_plane) noexcept;
```

The camera's rendering distance limits.

### viewMatrix, projMatrix

```cpp
const glm::mat4 & get_view_matrix() const noexcept;
const glm::mat4 & get_proj_matrix() const noexcept;
```

The camera's view and projection matrices, calculated based on its position, direction and FOV.