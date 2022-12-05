# [assimp_skeleton_model](assimp_skeleton_model.hpp)

Holds AssImp-specific data for a [skeleton_model](../../../animation/skeleton_model.md).

## Members

### mesh type

```cpp
struct mesh {
	struct bone {
		const aiNode * node = nullptr;
		glm::mat4 offset{ 1.f };
	};
	std::vector<bone> bones;
};
```

Holds AssImp-specific data for a given mesh.

#### bone::node

The `aiNode` associated to the bone in the AssImp scene.

#### bone::offset

The bone's base transformation matrix.

#### bones

The list of bones in the mesh. Indexed according to [skeleton_model::mesh::bone_names](../skeleton_model.md).

### root_node

```cpp
const aiNode* root_node = nullptr;
```

The root node for this skeleton in the AssImp scene.

### assimp_meshes

```cpp
std::vector<mesh> assimp_meshes;
```

The list of meshes in the skeleton. Indexed according to [skeleton_model::meshes](../skeleton_model.md).