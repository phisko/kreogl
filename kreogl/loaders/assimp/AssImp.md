# [AssImp](AssImp.hpp)

Functions to load models and animations using the AssImp library.

## Members

### loadAnimatedModel

```cpp
std::unique_ptr<AnimatedModel> loadAnimatedModel(const char * file) noexcept;
```

Attemps to load a model from `file`.

### loadAnimationFile

```cpp
std::unique_ptr<AnimationFile> loadAnimationFile(const char * file) noexcept;
```

Attempts to load animations from `file`.