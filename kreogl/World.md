# [World](World.hpp)

Represents a game world.

## Members

### Collections

Worlds contain the following objects:
* [Object](Object.md)
* [DirectionalLight](lights/DirectionalLight.md)
* [PointLight](lights/PointLight.md)
* [SpotLight](lights/SpotLight.md)
* [DebugElement](debug/DebugElement.md)

For each of those types, the following functions are defined:

```cpp
void add(const T & object) noexcept;
void remove(const T & object) noexcept;
const std::vector<const T *> & getTs() const noexcept;
```

Note that the `add` functions ***do not store copies of the objects***. It is up to the client code to keep these objects alive until `remove` is called.

### getObjects

`getObjects()` is the only function with a different API:

```cpp
const std::vector<const Object *> & getObjects(const VertexSpecification & vertexSpecification) const noexcept;
```

This is because `Object`s are mapped according to their [model](model/Model.md)'s [vertex specification](impl/shaders/VertexSpecification.md).