# [SkeletonModel](SkeletonModel.hpp)

Holds [flyweight](https://en.wikipedia.org/wiki/Flyweight_pattern) information about a [Model](../model/Model.md)'s skeleton.

## Members

### Mesh type

Holds information about one of the model's meshes.

#### boneNames

```cpp
std::vector<std::string> boneNames;
```

The names of the bones in the mesh.

### meshes

```cpp
std::vector<Mesh> meshes;
```

The meshes composing the model, indexed according to [Model::meshes](../model/Model.md).