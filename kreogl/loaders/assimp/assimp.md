# [assimp](assimp.hpp)

Functions to load models and animations using the AssImp library.

## Members

### is_supported_file_format

```cpp
bool is_supported_file_format(const char * file) noexcept;
```

Returns whether `file` is a format that can be loaded by AssImp.

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

### load_model_data

```cpp
assimp_model_data load_model_data(const char * file) noexcept;
std::unique_ptr<animated_model> load_animated_model(assimp_model_data && model_data) noexcept;
```

Returns the intermediate data used when loading a model. `load_model_data` doesn't perform any OpenGL calls and can therefore be called from a loading thread, before later passing the data to `load_animated_model` on the main thread.