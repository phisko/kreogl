# [kreogl](kreogl.hpp)

Global state of the renderer.

## init

```cpp
bool initGLFW() noexcept;
bool initGlew() noexcept;
```

Initialize third-party libraries.

## createDefaultShaders

```cpp
void createDefaultShaders() noexcept;
```

Registers the provided default shaders.

## addShader

```cpp
void addShader(ShaderStep step, Shader & shader) noexcept;
```

Registers a [shader](shaders/Shader.md) for a certain [step](shaders/ShaderStep.md).

## runShaders

```cpp
void runShaders(ShaderStep step, const DrawParams & params) noexcept;
```

Runs all shaders for a certain step.

## fillShadowMap

```cpp
void fillShadowMap(const DirectionalLight & light, const DrawParams & params) noexcept;
void fillShadowMap(const SpotLight & light, const DrawParams & params) noexcept;
void fillShadowMap(const PointLight & light, const DrawParams & params) noexcept;
```

Fills the shadow map for `light`.