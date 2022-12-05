# [object](object.hpp)

Represents an in-game 3D object.

## Members

### model

```cpp
const model * model;
```

The [model](model/model.md) used by this object.

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

### user_data

```cpp
glm::vec4 user_data;
```

Custom user-provided data that will be drawn into the [gbuffer](impl/gbuffer.md)'s `user_data` texture.

### cast_shadows

```cpp
bool cast_shadows = true;
```

Whether or not this object should cast shadows, and thus be drawn into shadow maps.