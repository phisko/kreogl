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

```cpp
static const VertexSpecification skeletal;
```

Vertices for skeletal meshes. Attributes:
* 3-float position
* 3-float normal
* 2-float texture coordinates
* `KREOGL_BONE_INFO_PER_VERTEX`-float bone weights
* `KREOGL_BONE_INFO_PER_VERTEX`-unsigned int bone indices