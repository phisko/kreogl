# [Sprite](Sprite.hpp)

A sprite to be drawn into the [World](World.md). Two identical types exist: `Sprite2D` and `Sprite3D`. This allows them to be stored in separate collections in the [World](World.md), which is an optimization for the [SpriteShader](impl/shaders/gbuffer/Sprite/SpriteShader.md).

## Members

### texture

```cpp
const ImageTexture * texture = nullptr;
```

The [ImageTexture](texture/ImageTexture.md) to be drawn.

### transform

```cpp
glm::mat4 transform{ 1.f };
```

The transform matrix to be applied.

### color

```cpp
glm::vec4 color{ 1.f };
```

The color to be applied.

### userData

```cpp
glm::vec4 userData{ 0.f };
```

The user data to be drawn into the [GBuffer](impl/GBuffer.md).