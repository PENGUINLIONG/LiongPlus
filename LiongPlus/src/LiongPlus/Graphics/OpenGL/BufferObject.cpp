// File: BufferObject.cpp
// Author: Rendong Liang (Liong)
#include <utility>
#include "LiongPlus/Graphics/OpenGL/BufferObject.hpp"
#include "LiongPlus/Platform/MinOpenGL.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

_L_NS_BEG_DETAILS
using namespace LiongPlus::Platform::OpenGL;

constexpr unsigned
_aux_TranslateAccessPattern(unsigned accessPattern)
{
	if ((unsigned)BufferAccessPattern::FrequentRead == accessPattern)
		return GL_STATIC_DRAW;
	else if ((unsigned)BufferAccessPattern::FrequentReadWrite == accessPattern)
		return GL_DYNAMIC_DRAW;
	else // BufferAccessPattern::FrequentResize -or- OneOff
		return GL_STREAM_DRAW;
}
constexpr unsigned
_aux_GetAllocationSettings(bool mapThanSub, bool readable)
{
	if (mapThanSub)
		return GL_MAP_PERSISTENT_BIT |
		GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT |
		// Must be explicitly flushed.
		(readable ? GL_MAP_READ_BIT : 0);
	else
		return GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT |
		(readable ? GL_MAP_READ_BIT : 0);
}
constexpr unsigned
_aux_GetMappingSettings(bool readable)
{
	// Must be explicitly flushed.
	return GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT |
		(readable ? GL_MAP_READ_BIT : 0);
}

unsigned
_imp_BufferObject_Initialize()
{
	unsigned handle;
	if (glCreateBuffers)
		glCreateBuffers(1, &handle);
	else
		glGenBuffers(1, &handle);
	return handle;
}
void
_imp_BufferObject_Finalize(unsigned handle)
{
	if (handle)
		glDeleteBuffers(1, &handle);
}

void
_imp_BufferObject_Bind(unsigned handle, unsigned target)
{
	glBindBuffer(target, handle);
}
void
_imp_BufferObject_Unbind(unsigned target)
{
	glBindBuffer(target, 0);
}

void
_imp_BufferObject_BindIndex(unsigned handle, unsigned target, unsigned index, unsigned offset, unsigned size)
{
	glBindBufferRange(target, index, handle, offset, size);
}
void
_imp_BufferObject_UnbindIndex(unsigned target, unsigned index)
{
	if (target)
		glBindBufferRange(target, index, 0, 0, 0);
}

void
_imp_BufferObject_Allocate(unsigned handle, unsigned target, unsigned size, bool mapThanSub, bool readable)
{
	auto settings = _aux_GetAllocationSettings(mapThanSub, readable);
	if (glNamedBufferStorage)
		glNamedBufferStorage(handle, size, nullptr, settings);
	else if (glBufferStorage)
	{
		_imp_BufferObject_Bind(handle, target);
		glBufferStorage(target, size, nullptr, settings);
		_imp_BufferObject_Unbind(target);
	}
}
void
_imp_BufferObject_AllocateResizable(unsigned handle, unsigned target, unsigned size, unsigned accessPattern)
{
	auto accessCode = _aux_TranslateAccessPattern(accessPattern);
	if (glNamedBufferData)
		glNamedBufferData(handle, size, nullptr, accessCode);
	else
	{
		_imp_BufferObject_Bind(handle, target);
		glBufferData(target, size, nullptr, accessCode);
		_imp_BufferObject_Unbind(target);
	}
}

void*
_imp_BufferObject_Map(unsigned handle, unsigned target, unsigned offset, unsigned size, bool readable, bool mapThanSub)
{
	GLbitfield settings = _aux_GetMappingSettings(readable) |
		(mapThanSub ? GL_MAP_PERSISTENT_BIT : 0);
	if (glMapNamedBufferRange)
		return glMapNamedBufferRange(handle, offset, size, settings);
	else
	{
		_imp_BufferObject_Bind(handle, target);
		auto rv = glMapBufferRange(target, offset, size, settings);
		_imp_BufferObject_Unbind(target);
		return rv;
	}
}
void
_imp_BufferObject_Unmap(unsigned handle, unsigned target)
{
	if (glUnmapNamedBuffer)
		glUnmapNamedBuffer(handle);
	else
	{
		_imp_BufferObject_Bind(handle, target);
		glUnmapBuffer(target);
		_imp_BufferObject_Unbind(target);
	}
}
void
_imp_BufferObject_Flush(unsigned handle, unsigned target, unsigned offset, unsigned size)
{
	if (glFlushMappedNamedBufferRange)
		glFlushMappedNamedBufferRange(handle, offset, size);
	else
	{
		_imp_BufferObject_Bind(handle, target);
		glFlushMappedBufferRange(target, offset, size);
		_imp_BufferObject_Unbind(target);
	}
}

void
_imp_BufferObject_Write(unsigned handle, unsigned target, unsigned offset, unsigned size, const void* data)
{
	if (glNamedBufferSubData)
		glNamedBufferSubData(handle, offset, size, data);
	else
	{
		_imp_BufferObject_Bind(handle, target);
		glBufferSubData(target, offset, size, data);
		_imp_BufferObject_Unbind(target);
	}
}

_L_NS_END_DETAILS

_L_NS_END_GRAPHICS_OPENGL
