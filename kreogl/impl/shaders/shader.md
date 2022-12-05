# [shader](shader.hpp)

Interface for OpenGL shader programs.

## Members

### draw

```cpp
virtual void draw(const draw_params & params) noexcept = 0;
```

Runs the shader.

### init

```cpp
void init() noexcept;
```

Called by child classes' constructors.

### add_source_files

```cpp
virtual void add_source_files() noexcept = 0;
```

Calls `add_source_file` for each GLSL source shader.

### add_source_file

```cpp
void add_source_file(const char * glsl, GLenum type) noexcept;
```

Compiles the GLSL source shader so it can later be linked with the program.

### get_uniforms

```cpp
virtual std::vector<uniform_base *> get_uniforms() noexcept = 0;
```

Returns the list of [uniforms](uniform.md) used by the program.

### use

```cpp
UniformChecker use() noexcept;
```

Must be called by child classes before performing OpenGL operations. Returns an object which will check that all uniforms have been set by the time it is destroyed.

```cpp
void use_without_uniform_check() const noexcept;
```