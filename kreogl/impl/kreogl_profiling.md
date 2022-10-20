# [Profiling](kreogl_profiling.hpp)

Profiling macros, currently wrapping [Tracy](https://github.com/wolfpld/tracy). To enable profiling, set the `KREOGL_PROFILING` CMake variable to `ON`.

## Members

### KREOGL_PROFILING_SCOPE

Placed at the start of a function, instruments it.

### KREOGL_PROFILING_FRAME

Placed in client code to mark the end of a frame.