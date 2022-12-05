# [sample_cascaded_shadow_map_glsl](sample_cascaded_shadow_map_glsl.hpp)

Shadow map function implementations for [cascaded shadow maps](../../../../shadow_maps/cascaded_shadow_map.md).

```glsl
int get_cascade_index(vec3 world_pos);
vec2 get_shadow_map_value(vec3 world_pos);
float calc_shadow_with_csm(int index, vec3 world_pos, vec3 normal, vec3 light_dir);
float calc_shadow(vec3 world_pos, vec3 normal, vec3 light_dir);
```