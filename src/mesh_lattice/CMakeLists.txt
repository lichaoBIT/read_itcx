aux_source_directory(. src_DIR)

add_library(target_lib_mesh ${src_DIR})

target_include_directories(target_lib_mesh PUBLIC include)