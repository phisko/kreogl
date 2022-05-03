# [SampleShadowCubeGLSL](SampleShadowCubeGLSL.hpp)

Shadow map function implementations for [shadow cubes](../../../../shadowMaps/ShadowCube.md).

```glsl
vec2 getShadowMapValue(vec3 worldPos);
float calcShadow(vec3 worldPos, vec3 normal, vec3 lightDir); // lightDir is ignored
```