# [ModelData](ModelData.hpp)

Holds the data loaded for a [model](Model.md).

## Members

### release

```cpp
std::function<void()> release = nullptr;
```

User-provided function that will be called upon destruction, to release resources.

### meshes

```cpp
std::vector<MeshData> meshes;
```

The meshes contained in this model.

### vertexAttributes

```cpp
std::vector<VertexAttribute> vertexAttributes;
```

The list of vertex attributes for this model's meshes.

### vertexSize

```cpp
size_t vertexSize;
```

The size of the vertices in this model's meshes.

### MeshData

```cpp
struct MeshData {
	struct Buffer {
		size_t nbElements;
		size_t elementSize;
		const void * data;
	};

	Buffer vertices;
	Buffer indices;
	GLenum indexType;
};
```

### VertexAttribute

```cpp
struct VertexAttribute {
	const char * name;
	size_t offset;
	GLenum type;
	size_t elementCount;
};
```