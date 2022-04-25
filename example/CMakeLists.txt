cmake_minimum_required(VERSION 3.0)
project(kreogl_example)

file(GLOB imgui_src imgui/*.cpp)
add_executable(kreogl_example main.cpp ${imgui_src} imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
target_link_libraries(kreogl_example kreogl)
target_include_directories(kreogl_example PRIVATE imgui)