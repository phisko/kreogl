# [vertex_array](vertex_array.hpp)

RAII type for manipulating OpenGL vertex arrays. Calls `glGenVertexArrays` in its ctor and `glDeleteVertexArrays` in its dtor.