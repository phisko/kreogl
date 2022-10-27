# [DebugElement](DebugElement.hpp)

Can be added to the [World](World.md) to be drawn by the debug shader.

## Members

### Type enum

```cpp
enum class Type {
	Line,
	Sphere,
	Box
};
```

Lists the possible debug objects that may be drawn.

### type

```cpp
Type type;
```

The type of this debug element.

### transform

```cpp
glm::mat4 transform{ 1.f };
```

The transform to be applied to the debug element.

### color

```cpp
glm::vec4 color{ 1.f };
```

The color to be drawn.

### userData

```cpp
glm::vec4 userData{ 0.f };
```

The user data to be written to the [GBuffer](impl/GBuffer.md).

### lineStart, lineEnd

```cpp
glm::vec3 lineStart{ 0.f, 0.f, 0.f };
glm::vec3 lineEnd{ 0.f, 0.f, 1.f };
```

The start and end points (relative to `transform`) to be used when drawing a `Line`-type element.