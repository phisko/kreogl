# [shadow_map_shader](shadow_map_shader.hpp)

Interface for shaders used to fill [shadow maps](../../shadow_maps/shadow_map.md).

## Members

### draw_to_texture

```cpp
virtual void draw_to_texture(GLuint texture, const glm::mat4 & light_space_matrix, const draw_params & params) noexcept = 0;
```

Draws objects to the shadow map.

### draw

```cpp
void draw(const directional_light & light, const draw_params & params) noexcept;
void draw(const spot_light & light, const draw_params & params) noexcept;
```

Draws objects to `light`'s shadow map.