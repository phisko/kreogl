# [VertexSpecification](VertexSpecification.hpp)

Description of a vertex layout, for use by shaders.

## Members

### attributes

```cpp
struct Attribute {
	GLenum type;
	size_t elementCount;
};

std::vector<Attribute> attributes;
```

The list of attributes of the vertex.

## Pre-defined specifications

```cpp
static const VertexSpecification positionColor;
```

Vertices with a position and a color.