# [buffer](buffer.hpp)

RAII type for manipulating OpenGL buffers. Calls `glGenBuffers` in its ctor and `glDeleteBuffers` in its dtor.