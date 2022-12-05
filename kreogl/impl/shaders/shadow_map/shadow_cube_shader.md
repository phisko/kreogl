# [shadow_cube_shader](shadow_cube_shader.hpp)

Interface for shaders used to fill [shadow cubes](../../shadow_maps/shadow_cube.md).

## Members

### draw_to_texture

```cpp
virtual void draw_objects(const draw_params & params) noexcept = 0;
```

Draws objects to the shadow cube.

### draw

```cpp
void draw(const point_light & light, const draw_params & params) noexcept;
```

Draws objects to `light`'s shadow cube.