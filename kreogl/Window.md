# [Window](Window.hpp)

Class to create and manipulate a GLFW window.

## Members

### ConstructionParams

```cpp
const char * name = "kreogl";
glm::ivec2 size = { 1280, 720 };
bool resizable = false;
bool defaultShaders = true;
```

If `defaultShaders` is set to `false`, the engine's pre-built shaders will not be used, letting users provide their custom shaders.

### draw

```cpp
void draw(const class World & world) noexcept;
```

Draws the provided [world](World.md) into the window.

### shouldClose

```cpp
bool shouldClose() const noexcept;
```

Returns whether the close button was clicked.

### pollEvents

```cpp
static void pollEvents() noexcept;
```

Polls GLFW events for all windows.

### getDefaultCamera

```cpp
Camera & getDefaultCamera() noexcept { return *_defaultCamera; }
```

Returns the default camera, which gets automatically created with the window.

### addCamera

```cpp
void Window::addCamera(const Camera & camera) noexcept;
```

Adds a camera to the window.

### getGLFWWindow

```cpp
GLFWwindow * getGLFWwindow() const noexcept;
```

Returns the handle to the underlying GLFW window.

