message("Loading LiongPlus Core Functionalities")
set (LIONGPLUS_SOURCES ${LIONGPLUS_SOURCES}
	${CMAKE_CURRENT_SOURCE_DIR}/BufferPool.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DateTime.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Uri.cpp
)

add_subdirectory(FileFormat)
add_subdirectory(Graphics)
add_subdirectory(Media)
add_subdirectory(Net)
add_subdirectory(Platform)
add_subdirectory(Serialization)
add_subdirectory(Testing)
add_subdirectory(Threading)

set (LIONGPLUS_SOURCES ${LIONGPLUS_SOURCES} PARENT_SCOPE)
