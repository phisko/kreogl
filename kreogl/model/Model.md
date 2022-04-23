# [Model](Model.hpp)

Represents a 3D model, typically loaded from an asset, which will be used by one or multiple [objects](../Object.md).

## Members

### Constructor

```cpp
Model(const ModelData & data, VertexSpecification vertexSpecification) noexcept;
```

Constructs a model based on the pre-loaded [model data](ModelData.md).

### meshes

```cpp
std::vector<Mesh> meshes;
```

The meshes used by this model.

### vertexSpecification

```cpp
VertexSpecification vertexSpecification;
```

The [vertex specification](../impl/shaders/VertexSpecification.md) for this object's meshes.

### Mesh

#### Members

##### vertexArray

```cpp
VertexArray vertexArray;
```

The [vertex array object](../impl/RAII/VertexArray.md) used to draw this model.

##### vertexBuffer

```cpp
Buffer vertexBuffer;
```

The buffer containing the vertices for this mesh, bound to the vertex array.

##### indexBuffer

```cpp
Buffer indexBuffer;
```

The buffer containing the indices for this mesh, bound to the vertex array.

##### nbIndices

```cpp
size_t nbIndices;
```

The number of indices in this mesh.

##### indexType

```cpp
GLenum indexType = GL_UNSIGNED_INT;
```

The type of indices in this mesh.