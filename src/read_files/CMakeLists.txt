aux_source_directory(. src_DIR)

add_library(target_lib_read_files ${src_DIR})

target_include_directories(target_lib_read_files PUBLIC include)