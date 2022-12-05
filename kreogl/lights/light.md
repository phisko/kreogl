# [light](light.hpp)

The base type for all lights.

## Members

### Shadow map planes

```cpp
#ifndef KREOGL_SHADOW_MAP_NEAR_PLANE
# define KREOGL_SHADOW_MAP_NEAR_PLANE .1f
#endif
```

```cpp
#ifndef KREOGL_SHADOW_MAP_FAR_PLANE
# define KREOGL_SHADOW_MAP_FAR_PLANE 1000.f
#endif
```

The near and far plane values used by shadow map shaders.

### color

```cpp
glm::vec4 color;
```

The color emitted by the light.

### diffuse_strength

```cpp
float diffuse_strength = 1.f;
```

The intensity of this light's diffuse component.

### specular_strength

```cpp
float specular_strength = 1.f;
```

The intensity of this light's specular component.

### cast_shadows

```cpp
bool cast_shadows = true;
```

Whether this light should cast shadows or not.

### Shadow map parameters

```cpp
int shadow_pcf_samples = 1;
int shadow_map_size = 1024;
float shadowMapBias = .00001f;
```

The parameters used when drawing to this light's shadow map.

### Volumetric lighting parameters

```cpp
std::optional<volumetric_lighting_params> volumetric_lighting;
```

[Parameters](volumetric_lighting_params.md) for volumetric lighting. If set to `std::nullopt`, the light won't cast volumetric lighting.

### light_sphere_size

```cpp
float light_sphere_size = .25f;
```

Size of the light sphere representing this light.