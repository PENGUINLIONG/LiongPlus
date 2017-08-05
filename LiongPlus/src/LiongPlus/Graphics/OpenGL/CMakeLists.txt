report_using(USE_GRAPHICS "LiongPlus::Graphics::OpenGL")
if (USE_GRAPHICS_OPENGL)
	assert_using(OPENGL_ACC "OpenGL Accelerated Graphics")
	set(LIONGPLUS_SOURCES ${LIONGPLUS_SOURCES}
		${CMAKE_CURRENT_SOURCE_DIR}/BufferObject.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Sampler.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Shader.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/ShaderProgram.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/Texture.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/VertexArray.cpp
	)
endif()
set (LIONGPLUS_SOURCES ${LIONGPLUS_SOURCES} PARENT_SCOPE)
