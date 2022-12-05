# [debug_element](debug_element.hpp)

Can be added to the [world](world.md) to be drawn by the debug shader.

## Members

### Type enum

```cpp
enum class type {
	line,
	sphere,
	box
};
```

Lists the possible debug objects that may be drawn.

### type

```cpp
type type;
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

### user_data

```cpp
glm::vec4 user_data{ 0.f };
```

The user data to be written to the [gbuffer](impl/gbuffer.md).

### line_start, line_end

```cpp
glm::vec3 line_start{ 0.f, 0.f, 0.f };
glm::vec3 line_end{ 0.f, 0.f, 1.f };
```

The start and end points (relative to `transform`) to be used when drawing a `line`-type element.