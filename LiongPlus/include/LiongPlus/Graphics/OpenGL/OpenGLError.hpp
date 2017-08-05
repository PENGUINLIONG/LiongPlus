// File: OpenGLError.hpp
// Author: Rendong Liang (Liong)
#include "_"
#include "../../SafeError.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

enum OpenGLErrorCode
{
	Success_Perfect    = 0b0'000'0000,

	// The requested functionality is not supported, and cannot be
	//   simulated using other interfaces.
	Error_NotSupported = 0b1'000'0001,

	Error_Mask         = 0b1'000'0000,
};

using OpenGLError = SafeError<OpenGLErrorCode, unsigned char>;

_L_NS_END_GRAPHICS_OPENGL
