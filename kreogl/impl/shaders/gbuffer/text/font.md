# [font](font.hpp)

Holds the FreeType resource handles for a font (loaded from a given file, in a given size).

## Members

### Ctor

```cpp
font(const char * file, size_t size) noexcept;
```

Loads the font from `file` in the given `size`.

### Character struct

```cpp
struct Character {
	texture texture;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};
```

Represents a single character in the font. Holds the data loaded from FreeType, and the `texture` used to draw it.

### get_character

```cpp
const character * get_character(unsigned long c) noexcept;
```

Returns the `character` for `c`, loading it if necessary.

### get_size_and_generate_characters

```cpp
glm::vec2 get_size_and_generate_characters(const std::string & text, float scale_x, float scale_y) noexcept;
```

Generates all characters required for `text` and returns the size required to draw it, according to the given scale.