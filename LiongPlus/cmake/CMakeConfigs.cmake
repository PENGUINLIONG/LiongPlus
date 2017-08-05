# Turn on the modules you would use.
option(USE_FILEFORMAT "Use LiongPlus::FileFormat." ON)
option(USE_GRAPHICS "Use LiongPlus::Graphics." ON)
option(USE_GRAPHICS_OPENGL "Use LiongPlus::Graphics::OpenGL." ON)
option(USE_GRAPHICS_UI "Use LiongPlus::Graphics::UI." ON)
option(USE_GRAPHICS_UI_CONTROLS "Use LiongPlus::Graphics::UI::Controls." ON)
option(USE_MEDIA "Use LiongPlus::Media." ON)
option(USE_NET "Use LiongPlus::Net." OFF)
option(USE_NET_HTTP "Use LiongPlus::Net:Http." OFF)
option(USE_PLATFORM "Use LiongPlus::Platform." ON)
option(USE_SERIALIZATION "Use LiongPlus::Serialization." ON)
option(USE_TESTING "Use LiongPlus::Testing." ON)
option(USE_THREADING "Use LiongPlus::Threading." ON)

# GUI interface and accelerated graphics support.
option(OPENGL_ACC "Use OpenGL accelerated graphics." ON)
option(DIRECTX_ACC "Use DirectX accelerated graphics." OFF)

# OpenGL version
set(OPENGL_VERSION_MAJOR 3)
set(OPENGL_VERSION_MINOR 3)

if (${OPENGL_ACC})
	add_definitions(
		-D__L_OPENGL_VERSION_MAJOR=${OPENGL_VERSION_MAJOR}
		-D__L_OPENGL_VERSION_MINOR=${OPENGL_VERSION_MINOR}
	)
endif()
