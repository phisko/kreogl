# [model](model.hpp)

Represents a 3D model, typically loaded from an asset, which will be used by one or multiple [objects](../object.md).

## Members

### Constructor

```cpp
model(const model_data & data, vertex_specification vertex_specification) noexcept;
```

Constructs a model based on the pre-loaded [model data](model_data.md).

### meshes

```cpp
std::vector<mesh> meshes;
```

The meshes used by this model.

### vertex_specification

```cpp
vertex_specification vertex_specification;
```

The [vertex specification](../impl/shaders/vertex_specification.md) for this object's meshes.

### mesh

#### Members

##### vertex_array

```cpp
vertex_array vertex_array;
```

The [vertex array object](../impl/raii/vertex_array.md) used to draw this model.

##### vertex_buffer

```cpp
buffer vertex_buffer;
```

The buffer containing the vertices for this mesh, bound to the vertex array.

##### index_buffer

```cpp
buffer index_buffer;
```

The buffer containing the indices for this mesh, bound to the vertex array.

##### nb_indices

```cpp
size_t nb_indices;
```

The number of indices in this mesh.

##### index_type

```cpp
GLenum index_type = GL_UNSIGNED_INT;
```

The type of indices in this mesh.