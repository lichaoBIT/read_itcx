cmake_minimum_required(VERSION 3.16)

set(proj_name "read_itcx")
project(${proj_name}
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(eigen_root_dir C:/soft/eigen-3.4.0)
set(VTK_DIR C:/soft/vtk-prefix/lib/cmake)
set(gmsh_dir C:/soft/gmsh-prefix)

# Eigen3::Eigen
set(EIGEN3_DIR ${eigen_root_dir}/install-me/share/eigen3/cmake)
set(Eigen3_DIR ${eigen_root_dir}/install-me/share/eigen3/cmake)
if (NOT IS_DIRECTORY ${eigen_root_dir}/install-me)
    execute_process(COMMAND cmake -S ${eigen_root_dir} -B ${eigen_root_dir}/build -D CMAKE_INSTALL_PREFIX=${eigen_root_dir}/install-me)
    execute_process(COMMAND cmake --build ${eigen_root_dir}/build)
    execute_process(COMMAND cmake --install ${eigen_root_dir}/build)
endif ()
message(STATUS "EIGEN3_DIR = ${EIGEN3_DIR}")
find_package(Eigen3 REQUIRED NO_MODULE)

# gmsh
add_library(target_interface_gmsh INTERFACE)

target_include_directories(target_interface_gmsh INTERFACE "${gmsh_dir}/include")
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(dll_debug ${gmsh_dir}/lib-debug/gmsh.dll)
    set(dll_release ${gmsh_dir}/lib-release/gmsh.dll)

    set(lib_debug ${gmsh_dir}/lib-debug/gmsh.lib ${gmsh_dir}/lib-debug/gmshd.lib)
    set(lib_release ${gmsh_dir}/lib-release/gmsh.lib)
    target_link_libraries(target_interface_gmsh INTERFACE
            "$<IF:$<CONFIG:Debug>,${lib_debug},${lib_release}>")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    file(GLOB gmsh_lib_files ${gmsh_dir}/*/libgmsh.so*)
    if (NOT gmsh_lib_files)
        message(FATAL_ERROR "I cannot find gmsh lib files.")
        return()
    else ()
        message(STATUS "location of gmsh_lib: ${gmsh_lib_files}")
    endif ()
    target_link_libraries(target_interface_gmsh INTERFACE ${gmsh_lib_files})
endif ()

# find vtk
#find_package(VTK REQUIRED QUIET PATHS "${VTK_DIR}")
add_library(target_interface_vtk INTERFACE)
find_package(VTK REQUIRED)
if (NOT VTK_FOUND)
      message(FATAL_ERROR "${VTK_NOT_FOUND_MESSAGE}")
else ()
      message(STATUS "VTK_LIBRARIES = ${VTK_LIBRARIES}")
      message(STATUS "VTK_INCLUDE_DIRS = ${VTK_INCLUDE_DIRS}")
      target_include_directories(target_interface_vtk INTERFACE ${VTK_INCLUDE_DIRS})
      target_link_libraries(target_interface_vtk INTERFACE ${VTK_LIBRARIES})
 endif ()

add_subdirectory(src)
add_executable(read_itcx main.cpp)

target_link_libraries(read_itcx PRIVATE 
    target_lib_mesh)
