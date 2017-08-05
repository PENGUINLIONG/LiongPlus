// File: MinOpenGL.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include <iostream>
#include <cstdint>
#ifdef __L_WINDOWS
#include "MinWindows.hpp"
#endif

_L_NS_BEG_PLATFORM

namespace OpenGL
{
#ifndef _MSC_VER
#define __stdcall __attribute__ ((__stdcall__))
#endif

	enum GLError
	{
		GL_INVALID_ENUM = 0x0500,
		GL_INVALID_VALUE = 0x0501,
		GL_INVALID_OPERATION = 0x0502,
		GL_STACK_OVERFLOW = 0x0503,
		GL_STACK_UNDERFLOW = 0x0504,
		GL_OUT_OF_MEMORY = 0x0505,
	};

	template<typename T> struct GLFunc;
	template<typename TReturn, typename ... TArgs>
	struct GLFunc<TReturn(TArgs ...)>
	{
	private:
		using func_type = TReturn(__stdcall*)(TArgs ...);
		func_type _Func;
	public:
		const char* Name;
		GLFunc() : _Func() { }
		operator bool() { return _Func != nullptr; }
		// This should be called only internally.
		void operator=(void* ptr) { _Func = (func_type)ptr; }
		TReturn operator()(TArgs ... args) {
			auto rv = _Func(args...);
#ifdef _DEBUG
			auto err = (GLError)glGetError();
			if (err)
				std::cout << Name << " " << err << std::endl;
#endif
			return std::move(rv);
		}
	};
	template<typename ... TArgs>
	struct GLFunc<void(TArgs ...)>
	{
	private:
		using func_type = void(__stdcall*)(TArgs ...);
		func_type _Func;
	public:
		const char* Name;
		GLFunc() : _Func() { }
		operator bool() { return _Func != nullptr; }
		// This should be called only internally.
		void operator=(void* ptr) { _Func = (func_type)ptr; }
		void operator()(TArgs ... args) {
			_Func(args...);
#ifdef _DEBUG
			auto err = (GLError)glGetError();
			if (err)
				std::cout << Name << " " << err << std::endl;
#endif
		}
	};



	enum
	{
		GL_COMPILE_STATUS = 0x8B81,
		GL_LINK_STATUS = 0x8B82,
		GL_INFO_LOG_LENGTH = 0x8B84,
	};

#ifdef __L_WINDOWS
	
	extern "C" __declspec(dllimport)
		Windows::Handle __stdcall wglCreateContext(Windows::Handle dc);
	extern "C" __declspec(dllimport)
		Windows::Bool  __stdcall wglDeleteContext(Windows::Handle glrc);
	extern "C" __declspec(dllimport)
		Windows::Proc  __stdcall wglGetProcAddress(const char* func);
	extern "C" __declspec(dllimport)
		Windows::Bool  __stdcall wglMakeCurrent(Windows::Handle dc, Windows::Handle glrc);

	// WGL_ARB_create_context
	enum
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB= 0x2091,
		WGL_CONTEXT_MINOR_VERSION_ARB= 0x2092,
		WGL_CONTEXT_FLAGS_ARB = 0x2094,
	};
	enum
	{
		WGL_CONTEXT_DEBUG_BIT_ARB = 0x0001,
		WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB = 0x0002,
	};
	extern GLFunc<Windows::Handle(Windows::Handle hDC, Windows::Handle shareContext, const int *attribList)> wglCreateContextAttribsARB;

	// WGL_ARB_pixel_format
	enum
	{
		WGL_DRAW_TO_WINDOW_ARB = 0x2001,
		WGL_SUPPORT_OPENGL_ARB = 0x2010,
		WGL_DOUBLE_BUFFER_ARB = 0x2011,
		WGL_PIXEL_TYPE_ARB = 0x2013,
		WGL_TYPE_RGBA_ARB = 0x202B,
		WGL_COLOR_BITS_ARB = 0x2014,
		WGL_DEPTH_BITS_ARB = 0x2022,
		WGL_STENCIL_BITS_ARB = 0x2023,
		WGL_SWAP_METHOD_ARB = 0x2007,
		WGL_SWAP_EXCHANGE_ARB = 0x2028,
	};
	extern GLFunc<Windows::Bool(Windows::Handle hdc,
		const int* iAttribIList,
		const float* fAttribFList,
		unsigned nMaxFormats,
		int* formats,
		unsigned* numFormats)> wglChoosePixelFormatARB;

	// WGL_ARB_multisample
	enum
	{
		WGL_SAMPLE_BUFFERS_ARB = 0x2041,
		WGL_SAMPLES_ARB = 0x2042,
	};
#endif

	// OpenGL Core 1.1
	using GLenum = unsigned;
	using GLuint = unsigned;
	using GLint = int;
	using GLsizei = int;
	using GLbitfield = unsigned int;
	using GLboolean = unsigned char;
	using GLfloat = float;
	using GLclampf = float;
	using GLvoid = void;
	using GLchar = char;
	enum {
		GL_ZERO = 0,
		GL_ONE = 1,
	};
	enum {
		GL_FALSE = 0,
		GL_TRUE = 1,
	};
	enum {
		GL_BYTE = 0x1400,
		GL_UNSIGNED_BYTE = 0x1401,
		GL_SHORT = 0x1402,
		GL_UNSIGNED_SHORT = 0x1403,
		GL_INT = 0x1404,
		GL_UNSIGNED_INT = 0x1405,
		GL_FLOAT = 0x1406,
	};
	enum
	{
		GL_RED = 0x1903,
		GL_GREEN = 0x1904,
		GL_BLUE = 0x1905,
		GL_ALPHA = 0x1906,
		GL_RGB = 0x1907,
		GL_RGBA = 0x1908,
	};
	enum
	{
		GL_POINTS = 0x0000,
		GL_LINES = 0x0001,
		GL_LINE_LOOP = 0x0002,
		GL_LINE_STRIP = 0x0003,
		GL_TRIANGLES = 0x0004,
		GL_TRIANGLE_STRIP = 0x0005,
		GL_TRIANGLE_FAN = 0x0006,
		GL_POLYGON_BIT = 0x00000008,
		GL_POLYGON = 0x0009,
	};
	enum
	{
		GL_TEXTURE_1D = 0x0DE0,
		GL_TEXTURE_2D = 0x0DE1,
	};
	enum
	{
		GL_TEXTURE_MAG_FILTER = 0x2800,
		GL_TEXTURE_MIN_FILTER = 0x2801,
		
		GL_TEXTURE_WRAP_S = 0x2802,
		GL_TEXTURE_WRAP_T = 0x2803,
	};
	enum
	{
		GL_NEAREST = 0x2600,
		GL_LINEAR = 0x2601,
		GL_NEAREST_MIPMAP_NEAREST = 0x2700,
		GL_LINEAR_MIPMAP_NEAREST = 0x2701,
		GL_NEAREST_MIPMAP_LINEAR = 0x2702,
		GL_LINEAR_MIPMAP_LINEAR = 0x2703,
	};
	enum
	{
		GL_CLAMP = 0x2900,
		GL_REPEAT = 0x2901,
	};
	enum
	{
		GL_BLEND = 0x0BE2,
		GL_MULTISAMPLE = 0x809D,
	};
	enum
	{
		GL_SRC_COLOR = 0x0300,
		GL_ONE_MINUS_SRC_COLOR = 0x0301,
		GL_SRC_ALPHA = 0x0302,
		GL_ONE_MINUS_SRC_ALPHA = 0x0303,
		GL_DST_ALPHA = 0x0304,
		GL_ONE_MINUS_DST_ALPHA = 0x0305,
		GL_DST_COLOR = 0x0306,
		GL_ONE_MINUS_DST_COLOR = 0x0307,
	};
	enum
	{
		GL_DEPTH_BUFFER_BIT = 0x00000100,
		GL_STENCIL_BUFFER_BIT = 0x00000400,
		GL_COLOR_BUFFER_BIT = 0x00004000,
	};
	enum // Polygon fill mode.
	{
		GL_POINT = 0x1B00,
		GL_LINE = 0x1B01,
		GL_FILL = 0x1B02,

		GL_FRONT_LEFT = 0x0400,
		GL_FRONT_RIGHT = 0x0401,
		GL_BACK_LEFT = 0x0402,
		GL_BACK_RIGHT = 0x0403,
		GL_FRONT = 0x0404,
		GL_BACK = 0x0405,
		GL_LEFT = 0x0406,
		GL_RIGHT = 0x0407,
		GL_FRONT_AND_BACK = 0x0408,
	};
	extern "C" __declspec(dllimport)
		void __stdcall glBindTexture(GLenum target, GLuint texture);
	extern "C" __declspec(dllimport)
		void __stdcall glBlendFunc(GLenum sfactor, GLenum dfactor);
	extern "C" __declspec(dllimport)
		void __stdcall glClear(GLbitfield mask);
	extern "C" __declspec(dllimport)
		void __stdcall glClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	extern "C" __declspec(dllimport)
		void __stdcall glDeleteTextures(GLsizei n, const GLuint *textures);
	extern "C" __declspec(dllimport)
		void __stdcall glDisable(GLenum cap);
	extern "C" __declspec(dllimport)
		void __stdcall glDrawArrays(GLenum mode, GLint first, GLsizei count);
	extern "C" __declspec(dllimport)
		void __stdcall glDrawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);
	extern "C" __declspec(dllimport)
		void __stdcall glEnable(GLenum cap);
	extern "C" __declspec(dllimport)
		void __stdcall glGenTextures(GLsizei n, GLuint* textures);
	extern "C" __declspec(dllimport)
		unsigned __stdcall glGetError();
	extern "C" __declspec(dllimport)
		void __stdcall glPixelStoref(GLenum pname, GLfloat param);
	extern "C" __declspec(dllimport)
		void __stdcall glPixelStorei(GLenum pname, GLint param);
	extern "C" __declspec(dllimport)
		void __stdcall glPolygonMode(GLenum face, GLenum mode);
	extern "C" __declspec(dllimport)
		void __stdcall glTexImage1D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
	extern "C" __declspec(dllimport)
		void __stdcall glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
	extern "C" __declspec(dllimport)
		void __stdcall glTexParameteri(GLenum target, GLenum pname, GLint param);
	extern "C" __declspec(dllimport)
		void __stdcall glTexParameteriv(GLenum target, GLenum pname, const GLint *params);
	extern "C" __declspec(dllimport)
		void __stdcall glTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
	extern "C" __declspec(dllimport)
		void __stdcall glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
	extern "C" __declspec(dllimport)
		void __stdcall glViewport(GLint x, GLint y, GLsizei width, GLsizei height);

	// OpenGL Core 1.2
	enum
	{
		GL_TEXTURE_WRAP_R = 0x8072,
	};
	extern GLFunc<void(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels)> glTexImage3D;
	extern GLFunc<void(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)> glTexSubImage3D;

	// OpenGL Core 1.3
	enum
	{
		GL_TEXTURE0 = 0x84C0,
	};
	extern GLFunc<void(GLenum texture)> glActiveTexture;

	// OpenGL Core 1.5
	using GLintptr = ptrdiff_t;
	using GLsizeiptr = ptrdiff_t;
	enum
	{
		GL_ARRAY_BUFFER = 0x8892,
		GL_ELEMENT_ARRAY_BUFFER = 0x8893,
	};
	extern GLFunc<void(GLsizei n, const GLuint* buffers)> glDeleteBuffers;
	extern GLFunc<void(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)> glBufferSubData;
	extern GLFunc<void*(GLenum target, GLenum access)> glMapBuffer;
	extern GLFunc<void(GLenum target)> glUnmapBuffer;

	// OpenGL Core 2.0
	enum
	{
		GL_ACTIVE_ATTRIBUTES = 0x8B89,
	};
	enum
	{
		GL_FRAGMENT_SHADER = 0x8B30,
		GL_VERTEX_SHADER = 0x8B31,
	};
	extern GLFunc<void(GLuint program, GLuint shader)> glAttachShader;
	extern GLFunc<void(GLuint shader)> glCompileShader;
	extern GLFunc<GLuint(void)> glCreateProgram;
	extern GLFunc<GLuint(GLenum type)> glCreateShader;
	extern GLFunc<void(GLuint program)> glDeleteProgram;
	extern GLFunc<void(GLuint shader)> glDeleteShader;
	extern GLFunc<void(GLuint program, GLuint shader)> glDetachShader;
	extern GLFunc<void(GLuint index)> glDisableVertexAttribArray;
	extern GLFunc<void(GLuint index)> glEnableVertexAttribArray;
	extern GLFunc<void(GLuint shader, GLenum pname, GLint* param)> glGetShaderiv;
	extern GLFunc<void(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)> glGetShaderInfoLog;
	extern GLFunc<void(GLuint program)> glLinkProgram;
	extern GLFunc<void(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog)> glGetProgramInfoLog;
	extern GLFunc<void(GLuint program, GLenum pname, GLint* param)> glGetProgramiv;
	extern GLFunc<void(GLuint shader, GLsizei count, const GLchar *const* string, const GLint* length)> glShaderSource;
	extern GLFunc<GLint(GLuint program, const GLchar* name)> glGetUniformLocation;
	extern GLFunc<void(GLint location, GLint v0)> glUniform1i;
	extern GLFunc<void(GLuint program)> glUseProgram;
	extern GLFunc<void(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)> glVertexAttribPointer;

	// OpenGL Core 2.1
	enum
	{
		GL_PIXEL_PACK_BUFFER = 0x88EB,
		GL_PIXEL_UNPACK_BUFFER = 0x88EC,
	};

	// OpenGL Core 3.0
	enum
	{
		GL_RGBA32F = 0x8814,
		GL_TRANSFORM_FEEDBACK_BUFFER = 0x8C8E,
	};
	enum
	{
		GL_TEXTURE_1D_ARRAY = 0x8C18,
		GL_TEXTURE_2D_ARRAY = 0x8C1A,
	};

	// OpenGL Core 3.1
	enum
	{
		GL_TEXTURE_BUFFER = 0x8C2A,
	};
	extern GLFunc<void(GLenum mode, GLint first, GLsizei count, GLsizei primcount)> glDrawArraysInstanced;
	extern GLFunc<void(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount)> glDrawElementsInstanced;
	extern GLFunc<void(GLenum target, GLenum internalFormat, GLuint buffer)> glTexBuffer;

	// OpenGL Core 3.2
	enum
	{
		GL_GEOMETRY_SHADER = 0x8DD9,
	};

	// OpenGL Core 3.3
	extern GLFunc<void(GLuint index, GLuint divisor)> glVertexAttribDivisor;

	// GL_ARB_base_instance
	extern GLFunc<void(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance)> glDrawElementsInstancedBaseVertexBaseInstance;

	// GL_ARB_buffer_storage
	enum
	{
		GL_MAP_READ_BIT = 0x0001,
		GL_MAP_WRITE_BIT = 0x0002,
		GL_MAP_PERSISTENT_BIT = 0x00000040,
		GL_MAP_COHERENT_BIT = 0x00000080,
		GL_DYNAMIC_STORAGE_BIT = 0x0100,
	};
	extern GLFunc<void(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags)> glBufferStorage;
	extern GLFunc<void(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags)> glNamedBufferStorage;

	// GL_ARB_draw_indirect
	extern GLFunc<void(GLenum mode, const void *indirect)> glDrawArraysIndirect;
	extern GLFunc<void(GLenum mode, GLenum type, const void *indirect)> glDrawElementsIndirect;

	// GL_ARB_map_buffer_range
	enum
	{
		GL_MAP_FLUSH_EXPLICIT_BIT = 0x0010,
	};
	extern GLFunc<void(GLenum target, GLintptr offset, GLsizeiptr length)> glFlushMappedBufferRange;
	extern GLFunc<void*(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)> glMapBufferRange;

	// GL_ARB_multi_draw_indirect
	extern GLFunc<void(GLenum mode, const void *indirect, GLsizei primcount, GLsizei stride)> glMultiDrawArraysIndirect;
	extern GLFunc<void(GLenum mode, GLenum type, const void *indirect, GLsizei primcount, GLsizei stride)> glMultiDrawElementsIndirect;

	// GL_ARB_texture_storage
	extern GLFunc<void(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)> glTexStorage1D;
	extern GLFunc<void(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)> glTexStorage2D;
	extern GLFunc<void(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)> glTexStorage3D;
	extern GLFunc<void(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width)> glTextureStorage1D;
	extern GLFunc<void(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)> glTextureStorage2D;
	extern GLFunc<void(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)> glTextureStorage3D;


	// GL_ARB_tessellation_shader
	enum
	{
		GL_TESS_EVALUATION_SHADER = 0x8E87,
		GL_TESS_CONTROL_SHADER = 0x8E88,
	};

	// GL_ARB_vertex_shader
	extern GLFunc<GLint(GLuint program, const GLchar* name)> glGetAttribLocation;
	
	// GL_ARB_vertex_array_object
	extern GLFunc<void(GLuint array)> glBindVertexArray;
	extern GLFunc<void(GLsizei n, const GLuint* arrays)> glDeleteVertexArrays;
	extern GLFunc<void(GLsizei n, GLuint* arrays)> glGenVertexArrays;

	// GL_ARB_vertex_attrib_binding
	extern GLFunc<void(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)> glBindVertexBuffer;
	extern GLFunc<void(GLuint attribindex, GLuint bindingindex)> glVertexAttribBinding;
	extern GLFunc<void(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)> glVertexAttribFormat;
	extern GLFunc<void(GLuint bindingindex, GLuint divisor)> glVertexBindingDivisor;

	// GL_ARB_vertex_buffer_object
	using GLsizeiptrARB = ptrdiff_t;
	enum
	{
		GL_STREAM_DRAW = 0x88E0,
		GL_STREAM_READ = 0x88E1,
		GL_STREAM_COPY = 0x88E2,
		GL_STATIC_DRAW = 0x88E4,
		GL_STATIC_READ = 0x88E5,
		GL_STATIC_COPY = 0x88E6,
		GL_DYNAMIC_DRAW = 0x88E8,
		GL_DYNAMIC_READ = 0x88E9,
		GL_DYNAMIC_COPY = 0x88EA,
	};
	extern GLFunc<void(GLsizei n, GLuint* buffers)> glGenBuffers;
	extern GLFunc<void(GLenum target, GLuint buffer)> glBindBuffer;
	extern GLFunc<void(GLenum target, GLsizeiptrARB size, const void *data, GLenum usage)> glBufferData;
	
	// GL_ARB_uniform_buffer_object
	enum
	{
		GL_UNIFORM_BUFFER = 0x8A11,
		GL_UNIFORM_BLOCK_DATA_SIZE = 0x8A40,
	};
	extern GLFunc<void(GLenum target, GLuint index, GLuint buffer)> glBindBufferBase;
	extern GLFunc<void(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)> glBindBufferRange;
	extern GLFunc<void(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params)> glGetActiveUniformBlockiv;
	extern GLFunc<void(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName)> glGetActiveUniformName;
	extern GLFunc<GLuint(GLuint program, const GLchar* uniformBlockName)> glGetUniformBlockIndex;
	extern GLFunc<void(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)> glUniformBlockBinding;

	// GL_ARB_separate_shader_objects
	extern GLFunc<void(GLuint program, GLint location, GLint x)>glProgramUniform1i;

	// GL_ARB_shader_storage_buffer_object
	enum
	{
		GL_SHADER_STORAGE_BUFFER = 0x90D2,
	};

	// GL_ARB_program_interface_query
	enum
	{
		GL_UNIFORM = 0x92E1,
		GL_UNIFORM_BLOCK = 0x92E2,
		GL_PROGRAM_INPUT = 0x92E3,
		GL_PROGRAM_OUTPUT = 0x92E4,
		GL_BUFFER_VARIABLE = 0x92E5,

		GL_ACTIVE_RESOURCES = 0x92F5,
		GL_MAX_NAME_LENGTH = 0x92F6,
	};
	extern GLFunc<void(GLuint program, GLenum programInterface, GLenum pname, GLint* params)> glGetProgramInterfaceiv;

	// GL_ARB_ES2_compatibility
	extern GLFunc<void(void)> glReleaseShaderCompiler;

	// GL_ARB_sampler_objects
	extern GLFunc<void(GLuint unit, GLuint sampler)> glBindSampler;
	extern GLFunc<void(GLsizei count, const GLuint* samplers)> glDeleteSamplers;
	extern GLFunc<void(GLsizei count, GLuint* samplers)> glGenSamplers;

	// GL_ARB_texture_swizzle
	enum
	{
		GL_TEXTURE_SWIZZLE_R = 0x8E42,
		GL_TEXTURE_SWIZZLE_G = 0x8E43,
		GL_TEXTURE_SWIZZLE_B = 0x8E44,
		GL_TEXTURE_SWIZZLE_A = 0x8E45,
		GL_TEXTURE_SWIZZLE_RGBA = 0x8E46,
	};

	// GL_ARB_direct_state_access
	extern GLFunc<void(GLuint unit, GLuint texture)> glBindTextureUnit;
	extern GLFunc<void(GLsizei n, GLuint* buffers)> glCreateBuffers;
	extern GLFunc<void(GLsizei n, GLuint* framebuffers)> glCreateFrameBuffers;
	extern GLFunc<void(GLsizei n, GLuint* pipelines)> glCreateProgramPipelines;
	extern GLFunc<void(GLsizei n, GLuint* samplers)> glCreateSamplers;
	extern GLFunc<void(GLenum target, GLsizei n, GLuint* textures)> glCreateTextures;
	extern GLFunc<void(GLsizei n, GLuint* arrays)> glCreateVertexArrays;
	extern GLFunc<void(GLuint vaobj, GLuint index)> glEnableVertexArrayAttrib;
	extern GLFunc<void(GLuint vaobj, GLuint index)> glDisableVertexArrayAttrib;
	extern GLFunc<void(GLuint buffer, GLintptr offset, GLsizeiptr length)> glFlushMappedNamedBufferRange;
	extern GLFunc<void(GLuint texture)> glGenerateTextureMipmap;
	extern GLFunc<void(GLuint texture, GLenum pname, GLint param)> glTextureParameteri;
	extern GLFunc<void(GLuint texture, GLenum pname, const GLint* param)> glTextureParameteriv;
	extern GLFunc<void(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage)> glNamedBufferData;
	extern  GLFunc<void(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)> glVertexArrayAttribFormat;
	extern GLFunc<void*(GLuint buffer)> glMapNamedBuffer;
	extern GLFunc<void*(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)> glMapNamedBufferRange;
	extern GLFunc<void(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data)> glNamedBufferSubData;
	extern GLFunc<void(GLuint texture, GLenum internalFormat, GLuint buffer)> glTextureBuffer;
	extern GLFunc<void(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)> glTextureBufferRange;
	extern GLFunc<void(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels)> glTextureSubImage1D;
	extern GLFunc<void(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)> glTextureSubImage2D;
	extern GLFunc<void(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)> glTextureSubImage3D;
	extern GLFunc<void*(GLuint buffer)> glUnmapNamedBuffer;
	extern GLFunc<void*(GLuint vaobj, GLuint attribindex, GLuint bindingindex)> glVertexArrayAttribBinding;
	extern GLFunc<void(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)> glVertexArrayVertexBuffer;
	extern GLFunc<void(GLuint vaobj, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr *offsets, const GLsizei *strides)> glVertexArrayVertexBuffers;

	// GL_ARB_framebuffer_object
	extern GLFunc<void(GLenum target)> glGenerateMipmap;

	// GL_ARB_texture_buffer_range
	extern GLFunc<void(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)> glTexBufferRange;

	// GL_ARB_multi_bind
	extern GLFunc<void(GLenum target, GLuint first, GLsizei count, const GLuint* buffers)> glBindBuffersBase;
	extern GLFunc<void(GLenum target, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr *offsets, const GLsizeiptr *sizes)> glBindBuffersRange;
	extern GLFunc<void(GLuint first, GLsizei count, const GLuint* samplers)> glBindSamplers;
	extern GLFunc<void(GLuint first, GLsizei count, const GLuint* textures)> glBindTextures;
	extern GLFunc<void(GLuint first, GLsizei count, const GLuint* buffers, const GLintptr *offsets, const GLsizei *strides)> glBindVertexBuffers;

	// GL_ARB_shader_atomic_counters
	enum
	{
		GL_ATOMIC_COUNTER_BUFFER = 0x92C0,
	};

	// GL_ARB_multi_draw_indirect
	enum
	{
		GL_DRAW_INDIRECT_BUFFER = 0x8F3F,
	};

	// This should be called only internally.
	extern void _InitializeMinOpenGL();

#ifndef _MSC_VER
#undef __stdcall
#endif
}

_L_NS_END_PLATFORM

