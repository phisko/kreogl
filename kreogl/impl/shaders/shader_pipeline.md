# [shader_pipeline](shader_pipeline.hpp)

A pipeline of [shaders](shader.md) to run when drawing to a [window](../../window.md).

## Members

### add_shader

```cpp
void add_shader(shader_step step, shader & shader) noexcept;
```

Adds a shader to the specified [step](shader_step.md).

### run_shaders

```cpp
void run_shaders(shader_step step, const draw_params & params) const noexcept;
```

Runs all the shaders for the specified step.

### get_shaders

```cpp
const std::vector<shader *> * get_shaders(shader_step step) const noexcept;
```

Returns the shaders for the specified step, if any.