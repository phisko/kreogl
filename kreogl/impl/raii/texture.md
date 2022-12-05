# [texture](texture.hpp)

RAII type for manipulating OpenGL textures. Calls `glGenTextures` in its ctor and `glDeleteTextures` in its dtor.