#pragma once

#ifdef KREOGL_PROFILING
#include "tracy/Tracy.hpp"
#define KREOGL_PROFILING_SCOPE ZoneScoped
#define KREOGL_PROFILING_FRAME FrameMark
#else
#define KREOGL_PROFILING_SCOPE
#define KREOGL_PROFILING_FRAME
#endif