# [point_light](point_light.hpp)

A point light, such as a light bulb.

## Members

### position

```cpp
glm::vec3 position;
```

The light's position in the [world](../world.md).

### Dissipation parameters

```cpp
float constant = 1.f;
float linear = .09f;
float quadratic = .032f;
```

The individual components of the quadratic equation used to calculate light intensity according to range.