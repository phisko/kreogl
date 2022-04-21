# [Shader](Shader.hpp)

Interface for OpenGL shader programs.

## Members

### draw

```cpp
virtual void draw(const DrawParams & params) noexcept = 0;
```

Runs the shader.

### init

```cpp
void init() noexcept;
```

Called by child classes' constructors.

### addSourceFiles

```cpp
virtual void addSourceFiles() noexcept = 0;
```

Calls `addSourceFile` for each GLSL source shader.

### addSourceFile

```cpp
void addSourceFile(const char * glsl, GLenum type) noexcept;
```

Compiles the GLSL source shader so it can later be linked with the program.

### getUniforms

```cpp
virtual std::vector<UniformBase *> getUniforms() noexcept = 0;
```

Returns the list of [uniforms](Uniform.md) used by the program.

### use

```cpp
void use() noexcept;
```

Must be called by child classes before performing OpenGL operations.