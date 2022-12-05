# [world](world.hpp)

Represents a game world.

## Members

### Collections

Worlds contain the following objects:
* [object](object.md)
* [directional_light](lights/directional_light.md)
* [point_light](lights/point_light.md)
* [spot_light](lights/spot_light.md)
* [debug_element](debug_element.md)
* [sprite_2d and sprite_3d](sprite.md)
* [text_2d and text_3d](text.md)

For each of those types, the following functions are defined:

```cpp
void add(const T & object) noexcept;
void remove(const T & object) noexcept;
const std::vector<const T *> & get_Ts() const noexcept;
```

Note that the `add` functions ***do not store copies of the objects***. It is up to the client code to keep these objects alive until `remove` is called.

### get_objects

`get_objects()` is the only function with a different API:

```cpp
const std::vector<const object *> & get_objects(const vertex_specification & vertex_specification) const noexcept;
```

This is because `object`s are mapped according to their [model](model/model.md)'s [vertex specification](impl/shaders/vertex_specification.md).

### skybox

```cpp
struct skybox {
	const skybox_texture * texture = nullptr;
	glm::vec4 color{ 1.f };
};
skybox skybox;
```

The skybox to draw for the world. Empty by default, to add a skybox simply set its `texture` attribute (see [skybox_texture](impl/texture/skybox_texture.md)).