# [SampleCascadedShadowMapGLSL](SampleCascadedShadowMapGLSL.hpp)

Shadow map function implementations for [cascaded shadow maps](../../../../shadowMaps/CascadedShadowMap.md).

```glsl
int getCascadeIndex(vec3 worldPos);
vec2 getShadowMapValue(vec3 worldPos);
float calcShadowWithCSM(int index, vec3 worldPos, vec3 normal, vec3 lightDir);
float calcShadow(vec3 worldPos, vec3 normal, vec3 lightDir);
```