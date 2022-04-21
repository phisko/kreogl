# [PointLight](PointLight.hpp)

A point light, such as a light bulb.

## Members

### position

```cpp
glm::vec3 position;
```

The light's position in the [world](../World.md).

### range

```cpp
float range = 1000.f;
```

The maximum range for this light.

### Dissipation parameters

```cpp
float constant = 1.f;
float linear = .09f;
float quadratic = .032f;
```

The individual components of the quadratic equation used to calculate light intensity according to range.