# [PolyVox](PolyVox.hpp)

Helper functions to load PolyVox models.

## Members

### loadModel

```cpp
template<typename VertexData>
Model loadModel(::PolyVox::RawVolume<VertexData> & volume) noexcept;
```

Builds a mesh from the volume and loads it into a model, assuming it has the [positionColor vertex specification](../../impl/shaders/VertexSpecification.md).


```cpp
template<typename VertexData>
Model loadModel(::PolyVox::RawVolume<VertexData> & volume, const VertexSpecification & vertexSpecification, const std::vector<size_t> & vertexAttributeOffsets) noexcept;
```

Builds a mesh from the volume and loads it into a model with the specified [vertex specification](../../impl/shaders/VertexSpecification.md) and attribute offsets.

```cpp
template<typename Mesh>
Model loadModel(const Mesh & mesh, const VertexSpecification & vertexSpecification, const std::vector<size_t> & vertexAttributeOffsets) noexcept;
```

Loads a PolyVox mesh into a model with the specified [vertex specification](../../impl/shaders/VertexSpecification.md) and attribute offsets.

