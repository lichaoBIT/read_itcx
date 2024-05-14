cmake_minimum_required(VERSION 3.16)

set(proj_name "model-check")
project(${proj_name}
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_executable(read_itcx main.cpp)

target_link_libraries(read_itcx PRIVATE 
    target_lib_read_files)
