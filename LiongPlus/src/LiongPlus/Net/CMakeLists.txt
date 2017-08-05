report_using(USE_NET "LiongPlus::Net")
if (USE_NET)
	set(LIONGPLUS_SOURCES ${LIONGPLUS_SOURCES}
		${CMAKE_CURRENT_SOURCE_DIR}/Socket.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/SocketAddress.cpp
	)
	add_subdirectory(Http)
endif()
set (LIONGPLUS_SOURCES ${LIONGPLUS_SOURCES} PARENT_SCOPE)
