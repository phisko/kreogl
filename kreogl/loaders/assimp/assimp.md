# [assimp](assimp.hpp)

Functions to load models and animations using the AssImp library.

## Members

### load_animated_model

```cpp
std::unique_ptr<animated_model> load_animated_model(const char * file) noexcept;
```

Attemps to load a model from `file`.

### load_animation_file

```cpp
std::unique_ptr<animation_file> load_animation_file(const char * file) noexcept;
```

Attempts to load animations from `file`.