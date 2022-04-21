# [Object](Object.hpp)

Represents an in-game 3D object.

## Members

### model

```cpp
const Model * model;
```

The [model](model/Model.md) used by this object.

### transform

```cpp
glm::mat4 transform;
```

The object's transformation matrix.

### color

```cpp
glm::vec4 color;
```

The object's color.

### userData

```cpp
glm::vec4 userData;
```

Custom user-provided data that will be drawn into the [gbuffer](impl/GBuffer.md)'s `userData` texture.