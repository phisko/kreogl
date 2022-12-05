# [volumetric_lighting_params](volumetric_lighting_params.hpp)

Parameters for volumetric lighting.

## Members

### scattering

```cpp
float scattering;
```

See [Alexandre Pestana's blog post](https://www.alexandre-pestana.com/volumetric-lights/), which served as one of the inspirations for this implementation.

### nb_steps

```cpp
int nb_steps;
```

The number of steps to take. For each step, the shader will test the position at `view_pos + ray_dir * step_length` to see if it is lit or not. The accumulated number of lit positions will determine how much volumetric light is added to the pixel.

### default_step_length

```cpp
float default_step_length;
```

The default `step_length` used for the formula described above, in the case where no object was seen in the pixel.

### intensity

```cpp
float intensity;
```

The intensity of the volumetric lighting.