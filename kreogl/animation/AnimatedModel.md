# [AnimatedModel](AnimatedModel.hpp)

A [model](../model/Model.md) with additional information (such as a [Skeleton](SkeletonModel.md)) to play animations.

## Members

### Vertex type

```cpp
struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	float boneWeights[KREOGL_BONE_INFO_PER_VERTEX] = { 0.f };
	unsigned int boneIDs[KREOGL_BONE_INFO_PER_VERTEX] = { std::numeric_limits<unsigned int>::max() };
};
```

Defines the vertex format for animated models.

### MeshTextures type

```cpp
struct MeshTextures {
	std::vector<kreogl::ImageTexture> diffuseTextures;
	glm::vec4 diffuseColor;

	std::vector<kreogl::ImageTexture> specularTextures;
	glm::vec4 specularColor;
};
```

Holds the textures required to draw an animated mesh.

### skeleton

```cpp
std::unique_ptr<SkeletonModel> skeleton = nullptr;
```

A pointer to the [SkeletonModel](SkeletonModel.md) for this model.

### meshTextures

```cpp
std::vector<MeshTextures> meshTextures;
```

The textures required to draw this model's meshes. The indices in this array match those of [Model::meshes](../model/Model.md).

### animations

```cpp
std::vector<std::unique_ptr<AnimationModel>> animations;
```

The animations included in the model file, stored as [AnimationModels](AnimationModel.md).