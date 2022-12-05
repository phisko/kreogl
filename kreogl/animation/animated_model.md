# [animated_model](animated_model.hpp)

A [model](../model/model.md) with additional information (such as a [skeleton](skeleton_model.md)) to play animations.

## Members

### vertex type

```cpp
struct vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coords;
	float bone_weights[KREOGL_BONE_INFO_PER_VERTEX] = { 0.f };
	unsigned int bone_ids[KREOGL_BONE_INFO_PER_VERTEX] = { std::numeric_limits<unsigned int>::max() };
};
```

Defines the vertex format for animated models.

### mesh_textures type

```cpp
struct mesh_textures {
	std::vector<kreogl::image_texture> diffuse_textures;
	glm::vec4 diffuse_color;

	std::vector<kreogl::image_texture> specular_textures;
	glm::vec4 specular_color;
};
```

Holds the textures required to draw an animated mesh.

### skeleton

```cpp
std::unique_ptr<skeleton_model> skeleton = nullptr;
```

A pointer to the [skeleton_model](skeleton_model.md) for this model.

### mesh_textures

```cpp
std::vector<mesh_textures> mesh_textures;
```

The textures required to draw this model's meshes. The indices in this array match those of [model::meshes](../model/model.md).

### animations

```cpp
std::vector<std::unique_ptr<animation_model>> animations;
```

The animations included in the model file, stored as [AnimationModels](animation_model.md).