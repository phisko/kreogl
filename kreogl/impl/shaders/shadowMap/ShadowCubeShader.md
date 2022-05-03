# [ShadowCubeShader](ShadowCubeShader.hpp)

Interface for shaders used to fill [shadow cubes](../../shadowMaps/ShadowCube.md).

## Members

### drawToTexture

```cpp
virtual void drawObjects(const DrawParams & params) noexcept = 0;
```

Draws objects to the shadow cube.

### draw

```cpp
void draw(const PointLight & light, const DrawParams & params) noexcept;
```

Draws objects to `light`'s shadow cube.