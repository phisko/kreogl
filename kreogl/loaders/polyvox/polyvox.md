# [polyvox](polyvox.hpp)

Helper functions to load PolyVox models.

## Members

### load_model

```cpp
template<typename vertex_data>
model load_model(::PolyVox::RawVolume<vertex_data> & volume) noexcept;
```

Builds a mesh from the volume and loads it into a model, assuming it has the [position_color vertex specification](../../impl/shaders/vertex_specification.md).


```cpp
template<typename vertex_data>
model load_model(::PolyVox::RawVolume<vertex_data> & volume, const vertex_specification & vertex_specification, const std::vector<size_t> & vertex_attribute_offsets) noexcept;
```

Builds a mesh from the volume and loads it into a model with the specified [vertex specification](../../impl/shaders/vertex_specification.md) and attribute offsets.

```cpp
template<typename Mesh>
model load_model(const Mesh & mesh, const vertex_specification & vertex_specification, const std::vector<size_t> & vertex_attribute_offsets) noexcept;
```

Loads a PolyVox mesh into a model with the specified [vertex specification](../../impl/shaders/vertex_specification.md) and attribute offsets.

