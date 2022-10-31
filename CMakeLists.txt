cmake_minimum_required(VERSION 3.0)
project(kreogl HOMEPAGE_URL "https://github.com/phisko/kreogl")

cmake_policy(VERSION 3.13) # options shouldn't clear variables

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
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
	kreogl/animation/*.cpp kreogl/animation/*.hpp
	kreogl/lights/*.cpp kreogl/lights/*.hpp
	kreogl/model/*.cpp kreogl/model/*.hpp
	kreogl/loaders/*.cpp kreogl/loaders/*.hpp
	kreogl/loaders/polyvox/*.cpp kreogl/loaders/polyvox/*.hpp
	kreogl/loaders/assimp/*.cpp kreogl/loaders/assimp/*.hpp
	kreogl/loaders/assimp/impl/*.cpp kreogl/loaders/assimp/impl/*.hpp
	kreogl/impl/*.cpp kreogl/impl/*.hpp
	kreogl/impl/texture/*.cpp kreogl/impl/texture/*.hpp
	kreogl/impl/shapes/*.cpp kreogl/impl/shapes/*.hpp
	kreogl/impl/shaders/*.cpp kreogl/impl/shaders/*.hpp
	kreogl/impl/shadowMaps/*.cpp kreogl/impl/shadowMaps/*.hpp)

# shaders
option(KREOGL_DEFAULT_SHADERS "Enable kreogl default shaders" ON)
if(KREOGL_DEFAULT_SHADERS)
	file(GLOB_RECURSE shadersSrc kreogl/impl/shaders/*/*.cpp kreogl/impl/shaders/*/*.hpp)
	set(src ${src} ${shadersSrc})
endif()

add_library(kreogl STATIC ${src})
target_include_directories(kreogl PUBLIC .)

if(KREOGL_DEFAULT_SHADERS)
	target_compile_definitions(kreogl PUBLIC KREOGL_DEFAULT_SHADERS)
endif()

# dependencies
set(customFindPackageNames glfw:glfw3)
set(customLibraryNames glew:GLEW)
putils_conan_download_and_link_packages_with_names(
	kreogl PUBLIC "${customFindPackageNames}" "${customLibraryNames}"
	glm/0.9.9.8
	assimp/5.0.1 # tested with 5.2.2 but that caused animation bugs (imported animation model was missing intermediate nodes)
	glew/2.2.0
	glfw/3.3.6
	stb/cci.20210910
	OPTIONS
		assimp:with_ifc=False # Otherwise AssImp will depend on clipper/4.10.0, which is broken
		glfw:shared=False
)

# find_package(freetype) is broken right now, so use the old cmake generator
# See https://github.com/conan-io/conan-center-index/issues/11218
putils_conan_download_packages(
	freetype/2.12.1
	GENERATORS
		cmake
)
include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
target_link_libraries(kreogl PUBLIC CONAN_PKG::freetype)

target_include_directories(kreogl PUBLIC third_party/polyvox/include)

# profiling
option(KREOGL_PROFILING OFF)
if(KREOGL_PROFILING)
	add_compile_definitions(KREOGL_PROFILING TRACY_ENABLE)

	include(FetchContent)
	FetchContent_Declare(
		tracy
		GIT_REPOSITORY https://github.com/wolfpld/tracy
		GIT_TAG v0.8.2.1 # Oct 15 2022
		GIT_PROGRESS TRUE
		GIT_SHALLOW TRUE
	)
	FetchContent_MakeAvailable(tracy)
	target_link_libraries(kreogl PUBLIC TracyClient)
endif()

option(KREOGL_EXAMPLE OFF)
if(KREOGL_EXAMPLE)
	add_subdirectory(example)
endif()