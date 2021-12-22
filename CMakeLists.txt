cmake_minimum_required(VERSION 3.16)
project(complex_file_system)

set(CMAKE_CXX_STANDARD 11)


include_directories(${PROJECT_SOURCE_DIR}/include)
#aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_MAIN_SRC)

file(GLOB root_header_files "${PROJECT_SOURCE_DIR}/include*.h")
file(GLOB root_src_cpp_files "${PROJECT_SOURCE_DIR}/src/*.cpp")

#add_executable(file_system ${root_src_files} ${root_src_files})
add_executable(file_system ${root_src_cpp_files} ${root_header_files})