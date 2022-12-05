# [text](text.hpp)

2D and 3D text elements. Two identical types exist: `text_2d` and `text_3d`. This allows them to be stored in separate collections in the [world](world.md), which is an optimization for the [text_shader](impl/shaders/gbuffer/text/text_shader.md).

## Members

### text

```cpp
std::string text;
```

The text to be displayed.

### font

```cpp
std::string font;
```

The font file to be loaded.

### font_size

```cpp
size_t font_size = 1024;
```

The font size to be used.

### alignment_type

```cpp
enum class alignment_type {
	left,
	center,
	right
};
alignment_type alignment = alignment_type::center;
```

The text alignment method to use.

### transform

```cpp
glm::mat4 transform{ 1.f };
```

The transform matrix to use.

### color

```cpp
glm::vec4 color{ 1.f };
```

The text's color.

### user_data

```cpp
glm::vec4 user_data{ 0.f };
```

The user data to be drawn into the [gbuffer](impl/gbuffer.md).