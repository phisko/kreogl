# [AssImpSkeletonModel](AssImpSkeletonModel.hpp)

Holds AssImp-specific data for a [SkeletonModel](../../../animation/SkeletonModel.md).

## Members

### Mesh type

```cpp
struct Mesh {
	struct Bone {
		const aiNode * node = nullptr;
		glm::mat4 offset{ 1.f };
	};
	std::vector<Bone> bones;
};
```

Holds AssImp-specific data for a given mesh.

#### Bone::node

The `aiNode` associated to the bone in the AssImp scene.

#### Bone::offset

The bone's base transformation matrix.

#### bones

The list of bones in the mesh. Indexed according to [SkeletonModel::Mesh::boneNames](../SkeletonModel.md).

### rootNode

```cpp
const aiNode* rootNode = nullptr;
```

The root node for this skeleton in the AssImp scene.

### assimpMeshes

```cpp
std::vector<Mesh> assimpMeshes;
```

The list of meshes in the skeleton. Indexed according to [SkeletonModel::meshes](../SkeletonModel.md).