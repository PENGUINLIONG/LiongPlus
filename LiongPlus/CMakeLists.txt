#
# Project: LiongPlus
#
project(LiongPlus)
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Load configs and configure dependencies.
include("cmake/CMakeConfigs.cmake")
include("cmake/FunctionDef.cmake")
include("cmake/CheckInternalDependency.cmake")

set(LiongPlus_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" PARENT_SCOPE)
set(LiongPlus_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src" PARENT_SCOPE)

if (USE_GRAPHICS_OPENGL )
	find_package(OpenGL REQUIRED)
	if (OPENGL_FOUND)
		include_directories(${OPENGL_INCLUDE_DIRS})
	endif()
endif()
if (USE_MEDIA)
	find_package(FreeType REQUIRED)
	if (FREETYPE_FOUND)
		include_directories(${FREETYPE_INCLUDE_DIRS})
	endif()
endif()
if (USE_FILEFORMAT)
	find_package(PNG REQUIRED)
	if (PNG_FOUND)
		include_directories(${PNG_INCLUDE_DIRS})
	endif()
endif()

set(LIONGPLUS_SOURCES "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(LiongPlus OBJECT ${LIONGPLUS_SOURCES})
