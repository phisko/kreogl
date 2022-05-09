# [VolumetricLightingParams](VolumetricLightingParams.hpp)

Parameters for volumetric lighting.

## Members

### scattering

```cpp
float scattering;
```

See [Alexandre Pestana's blog post](https://www.alexandre-pestana.com/volumetric-lights/), which served as one of the inspirations for this implementation.

### nbSteps

```cpp
float nbSteps;
```

The number of steps to take. For each step, the shader will test the position at `viewPos + rayDir * stepLength` to see if it is lit or not. The accumulated number of lit positions will determine how much volumetric light is added to the pixel.

### defaultStepLength

```cpp
float defaultStepLength;
```

The default `stepLength` used for the formula described above, in the case where no object was seen in the pixel.

### intensity

```cpp
float intensity;
```

The intensity of the volumetric lighting.