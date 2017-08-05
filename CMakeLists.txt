#
# Top-most CMakeLists.txt
#
cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib") 
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
    message(FATAL_ERROR "Build directory is the same as the source directory.")
endif()

if (DEFINED MSVC)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)
	add_definitions(/std:c++latest) # Use C++17.
endif()

set(LIONGPLUS_SOURCES)

add_subdirectory("${PROJECT_SOURCE_DIR}/LiongPlus")
add_subdirectory("${PROJECT_SOURCE_DIR}/Demo")

add_dependencies(Demo LiongPlus)
