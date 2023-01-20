cmake_minimum_required(VERSION 3.0)
project(kreogl HOMEPAGE_URL "https://github.com/phisko/kreogl")

cmake_policy(VERSION 3.13) # options shouldn't clear variables

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX")
endif()

add_subdirectory(cmake_helpers)

#
# Library
#
putils_src_files(
	src
	DIRECTORIES
		kreogl
		kreogl/animation
		kreogl/lights
		kreogl/model
		kreogl/loaders
		kreogl/loaders/polyvox
		kreogl/loaders/assimp
		kreogl/loaders/assimp/impl
		kreogl/impl
		kreogl/impl/texture
		kreogl/impl/shapes
		kreogl/impl/shaders
		kreogl/impl/shadow_maps
	EXTENSIONS
		cpp hpp
)

# shaders
option(KREOGL_DEFAULT_SHADERS "Enable kreogl default shaders" ON)
if(KREOGL_DEFAULT_SHADERS)
	file(GLOB_RECURSE shadersSrc kreogl/impl/shaders/*/*.cpp kreogl/impl/shaders/*/*.hpp)
	set(src ${src} ${shadersSrc})
endif()

add_library(kreogl ${src})
target_include_directories(kreogl PUBLIC .)

if(KREOGL_DEFAULT_SHADERS)
	target_compile_definitions(kreogl PUBLIC KREOGL_DEFAULT_SHADERS)
endif()

# dependencies
set(customFindPackageNames glfw:glfw3)
set(customLibraryNames glew:GLEW)
putils_conan_set_shared_options(options assimp glew glfw)
list(APPEND options assimp:with_ifc=False) # Otherwise AssImp will depend on clipper/4.10.0, which is broken
putils_conan_download_and_link_packages_with_names(
	kreogl PUBLIC "${customFindPackageNames}" "${customLibraryNames}"
	glm/0.9.9.8
	assimp/5.0.1 # tested with 5.2.2 but that caused animation bugs (imported animation model was missing intermediate nodes)
	glew/2.2.0
	glfw/3.3.6
	stb/cci.20210910
	${options}
)

# find_package(freetype) is broken right now, so use the old cmake generator
# See https://github.com/conan-io/conan-center-index/issues/11218
putils_conan_set_shared_options(options freetype)
putils_conan_download_packages(
	freetype/2.12.1
	${options}
	GENERATORS
		cmake
)
include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS NO_OUTPUT_DIRS)
target_link_libraries(kreogl PUBLIC CONAN_PKG::freetype)

target_include_directories(kreogl PUBLIC third_party/polyvox/include)

# Export the symbols if we're built as a DLL
putils_export_symbols(kreogl)

# profiling
option(KREOGL_PROFILING OFF)
if(KREOGL_PROFILING)
	target_compile_definitions(kreogl PUBLIC KREOGL_PROFILING TRACY_ENABLE)

	set(TRACY_STATIC OFF) # Link as a shared library
	include(FetchContent)
	FetchContent_Declare(
		tracy
		GIT_REPOSITORY https://github.com/wolfpld/tracy
		GIT_TAG v0.9
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