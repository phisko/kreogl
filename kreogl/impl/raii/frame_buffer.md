# [frame_buffer](frame_buffer.hpp)

RAII type for manipulating OpenGL framebuffers. Calls `glGenFramebuffers` in its ctor and `glDeleteFramebuffers` in its dtor.