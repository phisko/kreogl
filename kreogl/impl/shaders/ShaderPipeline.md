# [ShaderPipeline](ShaderPipeline.hpp)

A pipeline of [Shaders](Shader.md) to run when drawing to a [Window](../../Window.md).

## Members

### addShader

```cpp
void addShader(ShaderStep step, Shader & shader) noexcept;
```

Adds a shader to the specified [step](ShaderStep.md).

### runShaders

```cpp
void runShaders(ShaderStep step, const DrawParams & params) const noexcept;
```

Runs all the shaders for the specified step.

### getShaders

```cpp
const std::vector<Shader *> * getShaders(ShaderStep step) const noexcept;
```

Returns the shaders for the specified step, if any.