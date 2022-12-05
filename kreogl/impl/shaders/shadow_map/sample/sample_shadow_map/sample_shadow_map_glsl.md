# [sample_shadow_map_glsl](sample_shadow_map_glsl.hpp)

Shadow map function implementations for [shadow maps](../../../../shadow_maps/shadow_map.md).

```glsl
vec2 get_shadow_map_value(vec3 world_pos);
bool is_position_lit(vec3 world_pos, float bias);
float calc_shadow(vec3 world_pos, vec3 normal, vec3 light_dir);
```