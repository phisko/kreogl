# [spot_light](spot_light.hpp)

A spotlight.

## Members

Inherits from [point_light](point_light.md).

### direction

```cpp
glm::vec3 direction = { 0, -1, 0 };
```

The direction this light emits in.

### cut_off, outer_cut_off

```cpp
float cut_off = 1.f;
float outer_cut_off = 1.2f;
```

The cutoff angle of the light cone.

### shadow_map

```cpp
mutable shadow_map shadow_map;
```

The [shadow map](../impl/shadow_maps/shadow_map.md) used by this light.

### get_light_space_matrix

```cpp
glm::mat4 get_light_space_matrix(const draw_params & params) const noexcept;
```