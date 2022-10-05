# [Skeleton](Skeleton.hpp)

Holds the transformation matrices for an [Object](../Object.md)'s bones.

## Members

### Mesh type

```cpp
struct Mesh {
	// Indexed by SkeletonModel::Mesh::boneNames
	std::vector<glm::mat4> boneMatsBoneSpace;
	std::vector<glm::mat4> boneMatsMeshSpace;
};
```

Holds the bones for a specific mesh in the model.

#### Bone matrices

```cpp
std::vector<glm::mat4> boneMatsBoneSpace;
std::vector<glm::mat4> boneMatsMeshSpace;
```

The matrices for the mesh's bones, indexed according to [SkeletonModel::Mesh::boneNames](SkeletonModel.md).

### meshes

```cpp
std::vector<Mesh> meshes;
```

The meshes composing the object, indexed according to [SkeletonModel::meshes](SkeletonModel.md).