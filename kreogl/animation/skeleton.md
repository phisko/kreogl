# [skeleton](skeleton.hpp)

Holds the transformation matrices for an [object](../object.md)'s bones.

## Members

### mesh type

```cpp
struct mesh {
	// Indexed by skeleton_model::mesh::bone_names
	std::vector<glm::mat4> bone_mats_bone_space;
	std::vector<glm::mat4> bone_mats_mesh_space;
};
```

Holds the bones for a specific mesh in the model.

#### Bone matrices

```cpp
std::vector<glm::mat4> bone_mats_bone_space;
std::vector<glm::mat4> bone_mats_mesh_space;
```

The matrices for the mesh's bones, indexed according to [skeleton_model::mesh::bone_names](skeleton_model.md).

### meshes

```cpp
std::vector<mesh> meshes;
```

The meshes composing the object, indexed according to [skeleton_model::meshes](skeleton_model.md).