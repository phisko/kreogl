# [assimp_model_data](assimp_model_data.hpp)

Data required to create an AssImp animated model.

## assimp_mesh_data

Holds the data required to create a single mesh from a model.

### Members

### vertices, indices

```cpp
std::vector<animated_model::vertex> vertices;
std::vector<unsigned int> indices;
```

### Textures and colors

```cpp
std::vector<texture_data> diffuse_textures;
glm::vec4 diffuse_color = { 0.f, 0.f, 0.f, 0.f };

std::vector<texture_data> specular_textures;
glm::vec4 specular_color = { 0.f, 0.f, 0.f, 0.f };
```