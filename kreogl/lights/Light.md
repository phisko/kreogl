# [Light](Light.hpp)

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

### diffuseStrength

```cpp
float diffuseStrength = 1.f;
```

The intensity of this light's diffuse component.

### specularStrength

```cpp
float specularStrength = 1.f;
```

The intensity of this light's specular component.

### castShadows

```cpp
bool castShadows = true;
```

Whether this light should cast shadows or not.

### Shadow map parameters

```cpp
int shadowPCFSamples = 1;
int shadowMapSize = 1024;
float shadowMapBias = .00001f;
```

The parameters used when drawing to this light's shadow map.

### Volumetric lighting parameters

```cpp
std::optional<VolumetricLightingParams> volumetricLighting;
```

[Parameters](VolumetricLightingParams.md) for volumetric lighting. If set to `std::nullopt`, the light won't cast volumetric lighting.