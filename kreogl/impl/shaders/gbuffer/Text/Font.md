# [Font](Font.hpp)

Holds the FreeType resource handles for a font (loaded from a given file, in a given size).

## Members

### Ctor

```cpp
Font(const char * file, size_t size) noexcept;
```

Loads the font from `file` in the given `size`.

### Character struct

```cpp
struct Character {
	Texture texture;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};
```

Represents a single character in the font. Holds the data loaded from FreeType, and the `texture` used to draw it.

### getCharacter

```cpp
const Character * getCharacter(unsigned long c) noexcept;
```

Returns the `Character` for `c`, loading it if necessary.

### getSizeAndGenerateCharacters

```cpp
glm::vec2 getSizeAndGenerateCharacters(const std::string & text, float scaleX, float scaleY) noexcept;
```

Generates all characters required for `text` and returns the size required to draw it, according to the given scale.