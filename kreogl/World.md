# [World](World.hpp)

Represents a game world.

## Members

For all the following `add` functions, the object taken by reference is ***not stored as a copy***. It is up to the client code to keep these objects alive until `remove` is called.

### Objects

```cpp
void add(const Object & object) noexcept;
void remove(const Object & object) noexcept;
const std::vector<const Object *> & getObjects(const VertexSpecification & vertexSpecification) const noexcept;
```

The [objects](Object.md) in the world, mapped according to their [model](model/Model.md)'s [vertex specification](impl/shaders/VertexSpecification.md).

### Lights

#### Directional lights

```cpp
void add(const DirectionalLight & light) noexcept;
void remove(const DirectionalLight & light) noexcept;
const std::vector<const DirectionalLight *> & getDirectionalLights() const noexcept;
```

#### Point lights

```cpp
void add(const PointLight & light) noexcept;
void remove(const PointLight & light) noexcept;
const std::vector<const PointLight *> & getPointLights() const noexcept;
```

#### Spot lights

```cpp
void add(const SpotLight & light) noexcept;
void remove(const SpotLight & light) noexcept;
const std::vector<const SpotLight *> & getSpotLights() const noexcept;
```