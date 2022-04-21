# [SpotLight](SpotLight.hpp)

A spotlight.

## Members

Inherits from [PointLight](PointLight.md).

### direction

```cpp
glm::vec3 direction = { 0, -1, 0 };
```

The direction this light emits in.

### cutOff, outerCutOff

```cpp
float cutOff = 1.f;
float outerCutOff = 1.2f;
```

The cutoff angle of the light cone.

### shadowMap

```cpp
mutable ShadowMap shadowMap;
```

The [shadow map](../impl/shadowMaps/ShadowMap.md) used by this light.

### getLightSpaceMatrix

```cpp
glm::mat4 getLightSpaceMatrix(const DrawParams & params) const noexcept;
```