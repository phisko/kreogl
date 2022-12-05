# [assimp_to_glm](assimp_to_glm.hpp)

Helper functions to convert AssImp data structures to GLM.

## Members

### toglm

```cpp
glm::mat4 toglm(const aiMatrix4x4 & mat) noexcept;
glm::vec3 toglm(const aiVector3D & vec) noexcept;
glm::quat toglm(const aiQuaternion & quat) noexcept;
```