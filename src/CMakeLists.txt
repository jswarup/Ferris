# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)
project (ferris)
 
# the project's entire directory structure.

include_directories( ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-Wall --std=c++14")
add_subdirectory (cove)
add_subdirectory (rube)

set(ferris_source_files console/sisyphus)
target_link_libraries(cove cove)
add_executable(ferris ${ferris_source_files})