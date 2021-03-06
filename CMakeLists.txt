cmake_minimum_required(VERSION 3.0)
project(kreogl HOMEPAGE_URL "https://github.com/phisko/kreogl")

cmake_policy(VERSION 3.13) # options shouldn't clear variables

set(CMAKE_CXX_STANDARD 20)
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX")
endif()

#
# CMake helpers
#

add_subdirectory(cmake_helpers)
include(conan_helper)

#
# Library
#

file(GLOB src
		kreogl/*.cpp kreogl/*.hpp
		kreogl/lights/*.cpp kreogl/lights/*.hpp
		kreogl/model/*.cpp kreogl/model/*.hpp
		kreogl/shapes/*.cpp kreogl/shapes/*.hpp
		kreogl/impl/*.cpp kreogl/impl/*.hpp
		kreogl/impl/shadowMaps/*.cpp kreogl/impl/shadowMaps/*.hpp)

# shaders
option(KREOGL_NO_DEFAULT_SHADERS "Disable kreogl default shaders" OFF)
if (KREOGL_NO_DEFAULT_SHADERS)
	add_compile_definitions(KREOGL_NO_DEFAULT_SHADERS)
else()
	file(GLOB_RECURSE shadersSrc kreogl/impl/shaders/*.cpp kreogl/impl/shaders/*.hpp)
	set(src ${src} ${shadersSrc})
endif()

add_library(kreogl STATIC ${src})
target_include_directories(kreogl PUBLIC .)

# dependencies
putils_conan(
    glew/2.2.0
    glm/0.9.9.8
	glfw/3.3.6
    freetype/2.11.1
	OPTIONS glfw:shared=False
)
target_link_libraries(kreogl PUBLIC CONAN_PKG::glew CONAN_PKG::glm CONAN_PKG::glfw CONAN_PKG::freetype)

target_include_directories(kreogl PUBLIC third_party/polyvox/include)

option(KREOGL_EXAMPLE OFF)
if (KREOGL_EXAMPLE)
    add_subdirectory(example)
endif()
