# [ShaderStep](ShaderStep.hpp)

Enum listing the different steps of the shader pipeline.

## Members

### Deferred pipeline steps

```cpp
GBuffer
```

Shaders filling the GBuffer's textures.

```cpp
Lighting
```

Shaders filling the final framebuffer with the lit contents of the gbuffer.

```cpp
PostLighting
```

Shaders which require lighting information to have been drawn and affect the lighting of the scene.

```cpp
PostProcess
```

Final shaders which add post-processing effects to the screen.

### Shadow map steps

```cpp
ShadowMap
```

Shaders used to fill shadow maps.

```cpp
ShadowCube
```

Shaders used to fill shadow cubes (for point lights).