#---------------------------------------- CMakeLists.txt --------------------------------------------------------------------

cmake_minimum_required(VERSION 2.8)

# Workspace name
project(ferris)

#options [ be sure to remove CMakeCache.txt after editing these lines ]
OPTION(XD_DBG "Build Library with Malloc Debug option" ON)
OPTION(XD_MALLOCDBG "Build Library with Malloc Debug option" OFF) 

#------------------------------------------------------------------------------------------------------------------------------

set(SOURCE_PATH "${CMAKE_BINARY_DIR}/../../../delivery/")

#source path
set(SOURCE_PATH "${CMAKE_SOURCE_DIR}/../../../src/")

# Set default locations
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
 
#------------------------------------------------------------------------------------------------------------------------------
  
include_directories( 
    ${SOURCE_PATH} 
)

#------------------------------------------------------------------------------------------------------------------------------

# Compiler option 

if(XD_DBG)
    set(CONFIGURATION_NAME "Debug")
    add_definitions(-DXD_DBG)
    add_definitions(-g) 
    add_definitions(-Wall)
    SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -g")
else()
    set(CONFIGURATION_NAME "Release")
#	add_definitions(-O)  
    SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -O3")
endif()

add_definitions(-std=c++14) 
 
# Linker options
link_directories( ${TARARI_DIRECTORY}/lib )
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lpcap")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L. ")

add_subdirectory (${SOURCE_PATH}/cove ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/cove)
add_subdirectory (${SOURCE_PATH}/rube ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/rube)

set(ferris_source_files ${SOURCE_PATH}/console/sisyphus) 
add_executable(ferris ${ferris_source_files})

#------------------------------------------------------------------------------------------------------------------------------ 


#------------------------------------------------------------------------------------------------------------------------------


 
