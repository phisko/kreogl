# [Text](Text.hpp)

2D and 3D text elements. Two identical types exist: `Text2D` and `Text3D`. This allows them to be stored in separate collections in the [World](World.md), which is an optimization for the [TextShader](impl/shaders/gbuffer/Text/TextShader.md).

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

### fontSize

```cpp
size_t fontSize = 1024;
```

The font size to be used.

### alignment

```cpp
enum class Alignment {
	Left,
	Center,
	Right
};
Alignment alignment = Alignment::Center;
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

### userData

```cpp
glm::vec4 userData{ 0.f };
```

The user data to be drawn into the [GBuffer](impl/GBuffer.md).