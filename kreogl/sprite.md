# [sprite](sprite.hpp)

A sprite to be drawn into the [world](world.md). Two identical types exist: `sprite_2d` and `sprite_3d`. This allows them to be stored in separate collections in the [world](world.md), which is an optimization for the [sprite_shader](impl/shaders/gbuffer/sprite/sprite_shader.md).

## Members

### texture

```cpp
const texture * tex = nullptr;
```

The [texture](impl/raii/texture.md) to be drawn.

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

### user_data

```cpp
glm::vec4 user_data{ 0.f };
```

The user data to be drawn into the [gbuffer](impl/gbuffer.md).