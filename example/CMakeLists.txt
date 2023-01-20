cmake_minimum_required(VERSION 3.21)
project(kreogl_example)

# Simple
add_executable(kreogl_example_simple simple_main.cpp)
target_link_libraries(kreogl_example_simple kreogl)
putils_copy_dlls(kreogl_example_simple)

add_custom_command(TARGET kreogl_example_simple POST_BUILD COMMAND ${CMAKE_COMMAND} -E
        make_directory $<TARGET_FILE_DIR:kreogl_example_simple>/resources)
add_custom_command(TARGET kreogl_example_simple POST_BUILD COMMAND ${CMAKE_COMMAND} -E
        copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources $<TARGET_FILE_DIR:kreogl_example_simple>/resources)

# Complex
add_executable(kreogl_example_complex complex_main.cpp)
target_link_libraries(kreogl_example_complex kreogl)
putils_copy_dlls(kreogl_example_complex)

add_custom_command(TARGET kreogl_example_complex POST_BUILD COMMAND ${CMAKE_COMMAND} -E
        make_directory $<TARGET_FILE_DIR:kreogl_example_complex>/resources)
add_custom_command(TARGET kreogl_example_complex POST_BUILD COMMAND ${CMAKE_COMMAND} -E
        copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources $<TARGET_FILE_DIR:kreogl_example_complex>/resources)