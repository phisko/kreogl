# [AnimationFile](AnimationFile.hpp)

Holds the [AnimationModels](AnimationModel.md) that were loaded from a file.

## Members

### animations

```cpp
std::vector<std::unique_ptr<AnimationModel>> animations;
```

The list of animations contained in the file.