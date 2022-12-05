# [window](window.hpp)

Class to create and manipulate a GLFW window.

## Members

### construction_params

```cpp
const char * name = "kreogl";
glm::ivec2 size = { 1280, 720 };
bool resizable = false;
```

### draw

```cpp
void draw(const class world & world, const shader_pipeline & shader_pipeline = shader_pipeline::get_default_shaders()) noexcept;
```

Draws the provided [world](world.md) into the window using the provided [shader_pipeline](impl/shaders/shader_pipeline.md).

### display

```cpp
void display() const noexcept;
```

Presents the contents of the window to the screen, calling `glfwSwapBuffers`.

### should_close

```cpp
bool should_close() const noexcept;
```

Returns whether the close button was clicked.

### poll_events

```cpp
static void poll_events() noexcept;
```

Polls GLFW events for all windows.

### get_default_camera

```cpp
camera & get_default_camera() noexcept { return *_default_camera; }
```

Returns the default camera, which gets automatically created with the window.

### Cameras

```cpp
void add_camera(const camera & camera) noexcept;
void remove_camera(const camera & camera) noexcept;
const std::vector<const camera *> & get_cameras() const noexcept;
```

All cameras displayed in the window.

### getGLFWWindow

```cpp
GLFWwindow * get_glfw_window() const noexcept;
```

Returns the handle to the underlying GLFW window.

## Detailed draw functions

### prepare_for_draw

```cpp
void prepare_for_draw() noexcept;
```

Runs the premilinary steps to draw a new frame.

### draw_world_to_camera

```cpp
void draw_world_to_camera(const class world & world, const camera & camera, const shader_pipeline & shader_pipeline = shader_pipeline::get_default_shaders()) noexcept;
```

Draws `world` into `camera` and blits its viewport onto the screen.