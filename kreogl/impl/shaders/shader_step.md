# [shader_step](shader_step.hpp)

Enum listing the different steps of the shader pipeline.

## Members

### Deferred pipeline steps

```cpp
gbuffer
```

Shaders filling the gbuffer's textures.

```cpp
lighting
```

Shaders filling the final framebuffer with the lit contents of the gbuffer.

```cpp
post_lighting
```

Shaders which require lighting information to have been drawn and affect the lighting of the scene.

```cpp
post_process
```

Final shaders which add post-processing effects to the screen.

### Shadow map steps

```cpp
shadow_map
```

Shaders used to fill shadow maps.

```cpp
shadow_cube
```

Shaders used to fill shadow cubes (for point lights).