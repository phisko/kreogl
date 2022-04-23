# [DirectionalLight](DirectionalLight.hpp)

A directional light, such as the sun.

## Members

### direction

```cpp
glm::vec3 direction{ 0, -1, 0 };
```

The direction the light is coming from.

### ambientStrength

```cpp
float ambientStrength = 1.f;
```

The intensity of the ambient effect of this light.

### cascadeEnds

```cpp
std::vector<float> cascadeEnds = { 50 };
```

The bounds of this light's [cascaded shadow map](../impl/shadowMaps/CascadedShadowMap.md)'s individual cascades.

### shadowCasterMaxDistance

```cpp
float shadowCasterMaxDistance = 100.f;
```

The maximum distance from the [camera](../Camera.md) for which [objects](../Object.md) will be evaluated for shadow casting.

### cascadedShadowMap

```cpp
mutable CascadedShadowMap cascadedShadowMap;
```

The [cascaded shadow map](../impl/shadowMaps/CascadedShadowMap.md) used by this light.

### getCascadedShadowMapLightSpaceMatrix

```cpp
glm::mat4 getCascadedShadowMapLightSpaceMatrix(const DrawParams & params, size_t index) const noexcept;
```

Returns the light space matrix for an individual cascade.

