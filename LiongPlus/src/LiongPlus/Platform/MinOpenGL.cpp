// File: MinOpenGL.cpp
// Author: Rendong Liang (Liong)
#include "LiongPlus/Platform/MinOpenGL.hpp"

_L_NS_BEG_PLATFORM

namespace OpenGL
{
	// WGL_ARB_create_context
	GLFunc<Windows::Handle(Windows::Handle hDC, Windows::Handle shareContext, const int *attribList)> wglCreateContextAttribsARB;

	// WGL_ARB_pixel_format
	GLFunc<Windows::Bool(Windows::Handle hdc,
		const int* iAttribIList,
		const float* fAttribFList,
		unsigned nMaxFormats,
		int* formats,
		unsigned* numFormats)> wglChoosePixelFormatARB;



	GLFunc<void(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels)> glTexImage3D;
	GLFunc<void(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)> glTexSubImage3D;

	// OpenGL Core 1.3
	GLFunc<void(GLenum texture)> glActiveTexture;

	// OpenGL Core 1.5
	GLFunc<void(GLsizei n, const GLuint* buffers)> glDeleteBuffers;
	GLFunc<void(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)> glBufferSubData;
	GLFunc<void*(GLenum target, GLenum access)> glMapBuffer;
	GLFunc<void(GLenum target)> glUnmapBuffer;

	// OpenGL Core 2.0
	GLFunc<void(GLuint program, GLuint shader)> glAttachShader;
	GLFunc<void(GLuint shader)> glCompileShader;
	GLFunc<GLuint(void)> glCreateProgram;
	GLFunc<GLuint(GLenum type)> glCreateShader;
	GLFunc<void(GLuint program)> glDeleteProgram;
	GLFunc<void(GLuint shader)> glDeleteShader;
	GLFunc<void(GLuint program, GLuint shader)> glDetachShader;
	GLFunc<void(GLuint index)> glDisableVertexAttribArray;
	GLFunc<void(GLuint index)> glEnableVertexAttribArray;
	GLFunc<void(GLuint shader, GLenum pname, GLint* param)> glGetShaderiv;
	GLFunc<void(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog)> glGetShaderInfoLog;
	GLFunc<void(GLuint program)> glLinkProgram;
	GLFunc<void(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog)> glGetProgramInfoLog;
	GLFunc<void(GLuint program, GLenum pname, GLint* param)> glGetProgramiv;
	GLFunc<void(GLuint shader, GLsizei count, const GLchar *const* string, const GLint* length)> glShaderSource;
	GLFunc<GLint(GLuint program, const GLchar* name)> glGetUniformLocation;
	GLFunc<void(GLint location, GLint v0)> glUniform1i;
	GLFunc<void(GLuint program)> glUseProgram;
	GLFunc<void(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)> glVertexAttribPointer;

	// OpenGL Core 3.1
	GLFunc<void(GLenum mode, GLint first, GLsizei count, GLsizei primcount)> glDrawArraysInstanced;
	GLFunc<void(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount)> glDrawElementsInstanced;
	GLFunc<void(GLenum target, GLenum internalFormat, GLuint buffer)> glTexBuffer;

	// OpenGL Core 3.3
	GLFunc<void(GLuint index, GLuint divisor)> glVertexAttribDivisor;

	// GL_ARB_base_instance
	GLFunc<void(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance)> glDrawElementsInstancedBaseVertexBaseInstance;

	// GL_ARB_buffer_storage
	GLFunc<void(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags)> glBufferStorage;
	GLFunc<void(GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags)> glNamedBufferStorage;

	// GL_ARB_draw_indirect
	GLFunc<void(GLenum mode, const void *indirect)> glDrawArraysIndirect;
	GLFunc<void(GLenum mode, GLenum type, const void *indirect)> glDrawElementsIndirect;

	// GL_ARB_map_buffer_range
	GLFunc<void(GLenum target, GLintptr offset, GLsizeiptr length)> glFlushMappedBufferRange;
	GLFunc<void*(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)> glMapBufferRange;

	// GL_ARB_multi_draw_indirect
	GLFunc<void(GLenum mode, const void *indirect, GLsizei primcount, GLsizei stride)> glMultiDrawArraysIndirect;
	GLFunc<void(GLenum mode, GLenum type, const void *indirect, GLsizei primcount, GLsizei stride)> glMultiDrawElementsIndirect;

	// GL_ARB_texture_storage
	GLFunc<void(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)> glTexStorage1D;
	GLFunc<void(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)> glTexStorage2D;
	GLFunc<void(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)> glTexStorage3D;
	GLFunc<void(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width)> glTextureStorage1D;
	GLFunc<void(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)> glTextureStorage2D;
	GLFunc<void(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)> glTextureStorage3D;

	// GL_ARB_vertex_shader
	GLFunc<GLint(GLuint program, const GLchar* name)> glGetAttribLocation;

	// GL_ARB_vertex_array_object
	GLFunc<void(GLuint array)> glBindVertexArray;
	GLFunc<void(GLsizei n, const GLuint* arrays)> glDeleteVertexArrays;
	GLFunc<void(GLsizei n, GLuint* arrays)> glGenVertexArrays;

	// GL_ARB_vertex_attrib_binding
	GLFunc<void(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)> glBindVertexBuffer;
	GLFunc<void(GLuint attribindex, GLuint bindingindex)> glVertexAttribBinding;
	 GLFunc<void(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)> glVertexAttribFormat;
	GLFunc<void(GLuint bindingindex, GLuint divisor)> glVertexBindingDivisor;

	// GL_ARB_vertex_buffer_object
	GLFunc<void(GLsizei n, GLuint* buffers)> glGenBuffers;
	GLFunc<void(GLenum target, GLuint buffer)> glBindBuffer;
	GLFunc<void(GLenum target, GLsizeiptrARB size, const void *data, GLenum usage)> glBufferData;

	// GL_ARB_uniform_buffer_object
	GLFunc<void(GLenum target, GLuint index, GLuint buffer)> glBindBufferBase;
	GLFunc<void(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)> glBindBufferRange;
	GLFunc<void(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params)> glGetActiveUniformBlockiv;
	GLFunc<void(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName)> glGetActiveUniformName;
	GLFunc<GLuint(GLuint program, const GLchar* uniformBlockName)> glGetUniformBlockIndex;
	GLFunc<void(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)> glUniformBlockBinding;

	// GL_ARB_separate_shader_objects
	GLFunc<void(GLuint program, GLint location, GLint x)>glProgramUniform1i;

	// GL_ARB_program_interface_query
	GLFunc<void(GLuint program, GLenum programInterface, GLenum pname, GLint* params)> glGetProgramInterfaceiv;

	// GL_ARB_ES2_compatibility
	GLFunc<void(void)> glReleaseShaderCompiler;

	// GL_ARB_sampler_objects
	GLFunc<void(GLuint unit, GLuint sampler)> glBindSampler;
	GLFunc<void(GLsizei n, GLuint* samplers)> glCreateSamplers;
	GLFunc<void(GLsizei count, const GLuint* samplers)> glDeleteSamplers;
	GLFunc<void(GLsizei count, GLuint* samplers)> glGenSamplers;

	// GL_ARB_direct_state_access
	GLFunc<void(GLuint unit, GLuint texture)> glBindTextureUnit;
	GLFunc<void(GLsizei n, GLuint* buffers)> glCreateBuffers;
	GLFunc<void(GLsizei n, GLuint* framebuffers)> glCreateFrameBuffers;
	GLFunc<void(GLsizei n, GLuint* pipelines)> glCreateProgramPipelines;
	GLFunc<void(GLenum target, GLsizei n, GLuint* textures)> glCreateTextures;
	GLFunc<void(GLsizei n, GLuint* arrays)> glCreateVertexArrays;
	GLFunc<void(GLuint vaobj, GLuint index)> glEnableVertexArrayAttrib;
	GLFunc<void(GLuint vaobj, GLuint index)> glDisableVertexArrayAttrib;
	GLFunc<void(GLuint buffer, GLintptr offset, GLsizeiptr length)> glFlushMappedNamedBufferRange;
	GLFunc<void(GLuint texture)> glGenerateTextureMipmap;
	GLFunc<void(GLuint texture, GLenum pname, GLint param)> glTextureParameteri;
	GLFunc<void(GLuint texture, GLenum pname, const GLint* param)> glTextureParameteriv;
	GLFunc<void(GLuint buffer, GLsizeiptr size, const void *data, GLenum usage)> glNamedBufferData;
	 GLFunc<void(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)> glVertexArrayAttribFormat;
	GLFunc<void*(GLuint buffer)> glMapNamedBuffer;
	GLFunc<void*(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)> glMapNamedBufferRange;
	GLFunc<void(GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data)> glNamedBufferSubData;
	GLFunc<void(GLuint texture, GLenum internalFormat, GLuint buffer)> glTextureBuffer;
	GLFunc<void(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)> glTextureBufferRange;
	GLFunc<void(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels)> glTextureSubImage1D;
	GLFunc<void(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels)> glTextureSubImage2D;
	GLFunc<void(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels)> glTextureSubImage3D;
	GLFunc<void*(GLuint buffer)> glUnmapNamedBuffer;
	GLFunc<void*(GLuint vaobj, GLuint attribindex, GLuint bindingindex)> glVertexArrayAttribBinding;
	GLFunc<void(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)> glVertexArrayVertexBuffer;
	GLFunc<void(GLuint vaobj, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr *offsets, const GLsizei *strides)> glVertexArrayVertexBuffers;

	// GL_ARB_framebuffer_object
	GLFunc<void(GLenum target)> glGenerateMipmap;

	// GL_ARB_texture_buffer_range
	GLFunc<void(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)> glTexBufferRange;

	// GL_ARB_multi_bind
	GLFunc<void(GLenum target, GLuint first, GLsizei count, const GLuint* buffers)> glBindBuffersBase;
	GLFunc<void(GLenum target, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr *offsets, const GLsizeiptr *sizes)> glBindBuffersRange;
	GLFunc<void(GLuint first, GLsizei count, const GLuint* samplers)> glBindSamplers;
	GLFunc<void(GLuint first, GLsizei count, const GLuint* textures)> glBindTextures;
	GLFunc<void(GLuint first, GLsizei count, const GLuint* buffers, const GLintptr *offsets, const GLsizei *strides)> glBindVertexBuffers;

	void _InitializeMinOpenGL()
	{
		static bool isInitialized = false;
		if (isInitialized) return;

#define _L_INIT_GLFUNC(name) name.Name = #name; name = wglGetProcAddress(#name)

		// WGL_ARB_create_context
		_L_INIT_GLFUNC(wglCreateContextAttribsARB);

		// WGL_ARB_pixel_format
		_L_INIT_GLFUNC(wglChoosePixelFormatARB);

		// OpenGL Core 1.2
		_L_INIT_GLFUNC(glTexImage3D);
		_L_INIT_GLFUNC(glTexSubImage3D);
		
		// OpenGL Core 1.3
		_L_INIT_GLFUNC(glActiveTexture);

		// OpenGL Core 1.5
		_L_INIT_GLFUNC(glDeleteBuffers);
		_L_INIT_GLFUNC(glBufferSubData);
		_L_INIT_GLFUNC(glMapBuffer);
		_L_INIT_GLFUNC(glUnmapBuffer);

		// OpenGL Core 2.0
		_L_INIT_GLFUNC(glAttachShader);
		_L_INIT_GLFUNC(glCompileShader);
		_L_INIT_GLFUNC(glCreateProgram);
		_L_INIT_GLFUNC(glCreateShader);
		_L_INIT_GLFUNC(glDeleteProgram);
		_L_INIT_GLFUNC(glDeleteShader);
		_L_INIT_GLFUNC(glDetachShader);
		_L_INIT_GLFUNC(glDisableVertexAttribArray);
		_L_INIT_GLFUNC(glEnableVertexAttribArray);
		_L_INIT_GLFUNC(glGetShaderiv);
		_L_INIT_GLFUNC(glGetShaderInfoLog);
		_L_INIT_GLFUNC(glLinkProgram);
		_L_INIT_GLFUNC(glGetProgramInfoLog);
		_L_INIT_GLFUNC(glGetProgramiv);
		_L_INIT_GLFUNC(glShaderSource);
		_L_INIT_GLFUNC(glGetUniformLocation);
		_L_INIT_GLFUNC(glUniform1i);
		_L_INIT_GLFUNC(glUseProgram);
		_L_INIT_GLFUNC(glVertexAttribPointer);

		// OpenGL Core 3.1
		_L_INIT_GLFUNC(glDrawArraysInstanced);
		_L_INIT_GLFUNC(glDrawElementsInstanced);
		_L_INIT_GLFUNC(glTexBuffer);

		// OpenGL Core 3.3
		_L_INIT_GLFUNC(glVertexAttribDivisor);

		// GL_ARB_base_instance
		_L_INIT_GLFUNC(glDrawElementsInstancedBaseVertexBaseInstance);

		// GL_ARB_buffer_storage
		_L_INIT_GLFUNC(glBufferStorage);
		_L_INIT_GLFUNC(glNamedBufferStorage);

		// GL_ARB_draw_indirect
		_L_INIT_GLFUNC(glDrawArraysIndirect);
		_L_INIT_GLFUNC(glDrawElementsIndirect);

		// GL_ARB_map_buffer_range
		_L_INIT_GLFUNC(glFlushMappedBufferRange);
		_L_INIT_GLFUNC(glMapBufferRange);

		// GL_ARB_multi_draw_indirect
		_L_INIT_GLFUNC(glMultiDrawArraysIndirect);
		_L_INIT_GLFUNC(glMultiDrawElementsIndirect);

		// GL_ARB_texture_storage
		_L_INIT_GLFUNC(glTexStorage1D);
		_L_INIT_GLFUNC(glTexStorage2D);
		_L_INIT_GLFUNC(glTexStorage3D);
		_L_INIT_GLFUNC(glTextureStorage1D);
		_L_INIT_GLFUNC(glTextureStorage2D);
		_L_INIT_GLFUNC(glTextureStorage3D);

		// GL_ARB_vertex_shader
		_L_INIT_GLFUNC(glGetAttribLocation);

		// GL_ARB_vertex_array_object
		_L_INIT_GLFUNC(glBindVertexArray);
		_L_INIT_GLFUNC(glDeleteVertexArrays);
		_L_INIT_GLFUNC(glGenVertexArrays);

		// GL_ARB_vertex_attrib_binding
		_L_INIT_GLFUNC(glBindVertexBuffer);
		_L_INIT_GLFUNC(glVertexAttribBinding);
		_L_INIT_GLFUNC(glVertexAttribFormat);
		_L_INIT_GLFUNC(glVertexBindingDivisor);

		// GL_ARB_vertex_buffer_object
		_L_INIT_GLFUNC(glGenBuffers);
		_L_INIT_GLFUNC(glBindBuffer);
		_L_INIT_GLFUNC(glBufferData);

		// GL_ARB_uniform_buffer_object
		_L_INIT_GLFUNC(glBindBufferBase);
		_L_INIT_GLFUNC(glBindBufferRange);
		_L_INIT_GLFUNC(glGetActiveUniformBlockiv);
		_L_INIT_GLFUNC(glGetActiveUniformName);
		_L_INIT_GLFUNC(glGetUniformBlockIndex);
		_L_INIT_GLFUNC(glUniformBlockBinding);

		// GL_ARB_separate_shader_objects
		_L_INIT_GLFUNC(glProgramUniform1i);

		// GL_ARB_vertex_program
		_L_INIT_GLFUNC(glGetProgramiv);

		// GL_ARB_program_interface_query
		_L_INIT_GLFUNC(glGetProgramInterfaceiv);

		// GL_ARB_ES2_compatibility
		_L_INIT_GLFUNC(glReleaseShaderCompiler);

		// GL_ARB_sampler_objects
		_L_INIT_GLFUNC(glBindSampler);
		_L_INIT_GLFUNC(glCreateSamplers);
		_L_INIT_GLFUNC(glDeleteSamplers);
		_L_INIT_GLFUNC(glGenSamplers);

		// GL_ARB_texture_swizzle

		// GL_ARB_direct_state_access
		_L_INIT_GLFUNC(glBindTextureUnit);
		_L_INIT_GLFUNC(glCreateBuffers);
		_L_INIT_GLFUNC(glCreateFrameBuffers);
		_L_INIT_GLFUNC(glCreateProgramPipelines);
		_L_INIT_GLFUNC(glCreateSamplers);
		_L_INIT_GLFUNC(glCreateTextures);
		_L_INIT_GLFUNC(glCreateVertexArrays);
		_L_INIT_GLFUNC(glEnableVertexArrayAttrib);
		_L_INIT_GLFUNC(glDisableVertexArrayAttrib);
		_L_INIT_GLFUNC(glFlushMappedNamedBufferRange);
		_L_INIT_GLFUNC(glGenerateTextureMipmap);
		_L_INIT_GLFUNC(glTextureParameteri);
		_L_INIT_GLFUNC(glTextureParameteriv);
		_L_INIT_GLFUNC(glNamedBufferData);
		_L_INIT_GLFUNC(glVertexArrayAttribFormat);
		_L_INIT_GLFUNC(glMapNamedBuffer);
		_L_INIT_GLFUNC(glMapNamedBufferRange);
		_L_INIT_GLFUNC(glNamedBufferSubData);
		_L_INIT_GLFUNC(glTextureBuffer);
		_L_INIT_GLFUNC(glTextureBufferRange);
		_L_INIT_GLFUNC(glTextureSubImage1D);
		_L_INIT_GLFUNC(glTextureSubImage2D);
		_L_INIT_GLFUNC(glTextureSubImage3D);
		_L_INIT_GLFUNC(glUnmapNamedBuffer);
		_L_INIT_GLFUNC(glVertexArrayAttribBinding);
		_L_INIT_GLFUNC(glVertexArrayVertexBuffer);
		_L_INIT_GLFUNC(glVertexArrayVertexBuffers);

		// GL_ARB_framebuffer_object
		_L_INIT_GLFUNC(glGenerateMipmap);

		// GL_ARB_texture_buffer_range
		_L_INIT_GLFUNC(glTexBufferRange);

		// GL_ARB_multi_bind
		_L_INIT_GLFUNC(glBindBuffersBase);
		_L_INIT_GLFUNC(glBindBuffersRange);
		_L_INIT_GLFUNC(glBindSamplers);
		_L_INIT_GLFUNC(glBindTextures);
		_L_INIT_GLFUNC(glBindVertexBuffers);

#undef _L_INIT_GLFUNC

		isInitialized = true;
	}
}

_L_NS_END_PLATFORM
