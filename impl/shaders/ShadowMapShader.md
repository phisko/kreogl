# [ShadowMapShader](ShadowMapShader.hpp)

Interface for shaders used to fill [shadow maps](../shadowMaps/ShadowMap.md).

## Members

### drawToTexture

```cpp
virtual void drawToTexture(GLuint texture, const glm::mat4 & lightSpaceMatrix, const DrawParams & params) noexcept = 0;
```

Draws objects to the shadow map.

### draw

```cpp
void draw(const DirectionalLight & light, const DrawParams & params) noexcept;
void draw(const SpotLight & light, const DrawParams & params) noexcept;
```

Draws objects to `light`'s shadow map.