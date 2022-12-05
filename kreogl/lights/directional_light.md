# [directional_light](directional_light.hpp)

A directional light, such as the sun.

## Members

### direction

```cpp
glm::vec3 direction{ 0, -1, 0 };
```

The direction the light is coming from.

### ambient_strength

```cpp
float ambient_strength = 1.f;
```

The intensity of the ambient effect of this light.

### light_sphere_distance

```cpp
float light_sphere_distance = 500.f;
```

Distance at which the sphere representing this light should appear.

### cascade_ends

```cpp
std::vector<float> cascade_ends = { 50 };
```

The bounds of this light's [cascaded shadow map](../impl/shadow_maps/cascaded_shadow_map.md)'s individual cascades.

### shadow_caster_max_distance

```cpp
float shadow_caster_max_distance = 100.f;
```

The maximum distance from the [camera](../camera.md) for which [objects](../object.md) will be evaluated for shadow casting.

### cascaded_shadow_map

```cpp
mutable cascaded_shadow_map cascaded_shadow_map;
```

The [cascaded shadow map](../impl/shadow_maps/cascaded_shadow_map.md) used by this light.

### get_light_space_matrix_for_cascade

```cpp
glm::mat4 get_light_space_matrix_for_cascade(const draw_params & params, size_t index) const noexcept;
```

Returns the light space matrix for an individual cascade.

