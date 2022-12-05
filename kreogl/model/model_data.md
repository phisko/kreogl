# [model_data](model_data.hpp)

Holds the data loaded for a [model](model.md).

## Members

### release

```cpp
std::function<void()> release = nullptr;
```

User-provided function that will be called upon destruction, to release resources.

### meshes

```cpp
std::vector<mesh_data> meshes;
```

The meshes contained in this model.

### vertex_attributes

```cpp
std::vector<vertex_attribute> vertex_attributes;
```

The list of vertex attributes for this model's meshes.

### vertex_size

```cpp
size_t vertex_size;
```

The size of the vertices in this model's meshes.

### mesh_data

```cpp
struct mesh_data {
	struct buffer {
		size_t nb_elements;
		size_t element_size;
		const void * data;
	};

	buffer vertices;
	buffer indices;
	GLenum index_type;
};
```

### vertex_attribute

```cpp
struct vertex_attribute {
	const char * name;
	size_t offset;
	GLenum type;
	size_t element_count;
};
```