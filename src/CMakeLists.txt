add_subdirectory(read_files)
add_subdirectory(mesh_lattice)
target_link_libraries(target_lib_read_files PUBLIC 
    Eigen3::Eigen
)
target_link_libraries(target_lib_mesh PUBLIC 
    target_lib_read_files
    target_interface_gmsh
    target_interface_vtk
)