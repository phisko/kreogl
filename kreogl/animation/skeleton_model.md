# [skeleton_model](skeleton_model.hpp)

Holds [flyweight](https://en.wikipedia.org/wiki/Flyweight_pattern) information about a [model](../model/model.md)'s skeleton.

## Members

### mesh type

Holds information about one of the model's meshes.

#### bone_names

```cpp
std::vector<std::string> bone_names;
```

The names of the bones in the mesh.

### meshes

```cpp
std::vector<mesh> meshes;
```

The meshes composing the model, indexed according to [model::meshes](../model/model.md).