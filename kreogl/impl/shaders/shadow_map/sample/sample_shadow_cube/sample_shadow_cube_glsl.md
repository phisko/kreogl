# [sample_shadow_cube_glsl](sample_shadow_cube_glsl.hpp)

Shadow map function implementations for [shadow cubes](../../../../shadow_maps/shadow_cube.md).

```glsl
vec2 get_shadow_map_value(vec3 world_pos);
float calc_shadow(vec3 world_pos, vec3 normal, vec3 light_dir); // light_dir is ignored
```