#
# Project: Demo
#
project(Demo)
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

include_directories(${LiongPlus_INCLUDE_DIR})

add_executable(Demo
	Cualific.hpp
	#src.cpp
	pmxSrc.cpp
	MikuMikuDance.hpp
	MikuMikuDance.cpp
	Pmx.hpp
	Pmx.cpp
	TextureReader.hpp
	$<TARGET_OBJECTS:LiongPlus>)

# ------
# Remenber to link dependencies in your own subproject.
# ------
# Declare DPI awareness.
if (${USE_GRAPHICS_UI} AND WIN32)
	target_link_libraries(Demo "User32.lib" "Shcore.lib")
endif()
# OpenGL as neccessary.
if (USE_GRAPHICS_OPENGL)
	find_package(OpenGL REQUIRED)
	if (OPENGL_FOUND)
		target_link_libraries(Demo ${OPENGL_LIBRARIES})
	endif()
	# Required.
	if (WIN32)
		target_link_libraries(Demo "winmm.lib")
	endif()
endif()
# FreeType is required for font rendering.
if (USE_MEDIA)
	find_package(FreeType REQUIRED)
	if (FREETYPE_FOUND)
		target_link_libraries(Demo ${FREETYPE_LIBRARIES})
	endif()
endif()
if (USE_FILEFORMAT)
	find_package(PNG REQUIRED)
	if (PNG_FOUND)
		target_link_libraries(Demo ${PNG_LIBRARIES})
	endif()
endif()
# ------
