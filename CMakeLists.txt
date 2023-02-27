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
find_package(assimp 5.0.1 CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
target_link_libraries(
	kreogl PUBLIC
	assimp::assimp
	Freetype::Freetype
	GLEW::GLEW
	glfw
	glm::glm
)

if(UNIX)
	# assimp doesn't link against IrrXML, see https://github.com/microsoft/vcpkg/issues/9918
	find_library(ASSIMP_IRRXML_LIBRARY IrrXML)
	target_link_libraries(kreogl PUBLIC ${ASSIMP_IRRXML_LIBRARY})
endif()

find_package(Stb REQUIRED)
target_include_directories(kreogl PUBLIC ${Stb_INCLUDE_DIR})

target_include_directories(kreogl PUBLIC third_party/polyvox/include)

# Export the symbols if we're built as a DLL
putils_export_symbols(kreogl)

# profiling
option(KREOGL_PROFILING "Enable profiling with Tracy")
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

option(KREOGL_EXAMPLE "Build examples")
if(KREOGL_EXAMPLE)
	add_subdirectory(example)
endif()