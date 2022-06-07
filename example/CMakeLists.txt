cmake_minimum_required(VERSION 3.0)
project(kreogl_example)

add_executable(kreogl_example main.cpp)
target_link_libraries(kreogl_example kreogl)

add_custom_command(TARGET kreogl_example POST_BUILD COMMAND ${CMAKE_COMMAND} -E
        make_directory $<TARGET_FILE_DIR:kreogl_example>/resources)
add_custom_command(TARGET kreogl_example POST_BUILD COMMAND ${CMAKE_COMMAND} -E
        copy_directory ${CMAKE_CURRENT_LIST_DIR}/resources $<TARGET_FILE_DIR:kreogl_example>/resources)