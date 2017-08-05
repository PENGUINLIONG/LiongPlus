// File: Texture.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <utility>
#include "LiongPlus/Graphics/OpenGL/Texture.hpp"
#include "LiongPlus/Platform/MinOpenGL.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

using namespace Platform::OpenGL;
using namespace Media;

_L_NS_BEG_DETAILS
constexpr size_t
_aux_MapPixelSize(Media::PixelFormat format) noexcept
{
	using Fmt = Media::PixelFormat;
	switch (format)
	{
		case Fmt::Alpha:
		case Fmt::Red:
		case Fmt::Green:
		case Fmt::Blue:
			return 1;
		case Fmt::Rgb:
		case Fmt::Bgr:
			return 3;
		case Fmt::Rgba:
			return 4;
	}
}
constexpr GLenum
_aux_MapPixelFormat(Media::PixelFormat format) noexcept
{
	switch (_aux_MapPixelSize(format))
	{
		case 1: return GL_RED;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
	}
}

inline void
_aux_GetRearrangedSwizzle(Media::PixelFormat format, int swizzle[4])
{
	switch (format)
	{
		case LiongPlus::Media::Alpha:
			swizzle[0] = GL_ONE;
			swizzle[1] = GL_ONE;
			swizzle[2] = GL_ONE;
			swizzle[3] = GL_RED;
			break;
		case LiongPlus::Media::Red:
			swizzle[0] = GL_RED;
			swizzle[1] = GL_ZERO;
			swizzle[2] = GL_ZERO;
			swizzle[3] = GL_ONE;
			break;
		case LiongPlus::Media::Green:
			swizzle[0] = GL_ZERO;
			swizzle[1] = GL_RED;
			swizzle[2] = GL_ZERO;
			swizzle[3] = GL_ONE;
			break;
		case LiongPlus::Media::Blue:
			swizzle[0] = GL_ZERO;
			swizzle[1] = GL_ZERO;
			swizzle[2] = GL_RED;
			swizzle[3] = GL_ONE;
			break;
		case LiongPlus::Media::Rgb:
			swizzle[0] = GL_RED;
			swizzle[1] = GL_GREEN;
			swizzle[2] = GL_BLUE;
			swizzle[3] = GL_ONE;
			break;
		case LiongPlus::Media::Bgr:
			swizzle[0] = GL_BLUE;
			swizzle[1] = GL_GREEN;
			swizzle[2] = GL_RED;
			swizzle[3] = GL_ONE;
			break;
		case LiongPlus::Media::Rgba:
			swizzle[0] = GL_RED;
			swizzle[1] = GL_GREEN;
			swizzle[2] = GL_BLUE;
			swizzle[3] = GL_ALPHA;
			break;
	}
}



unsigned
_imp_Texture_Initialize(unsigned target)
{
	unsigned handle;
	if (glCreateTextures)
		glCreateTextures(target, 1, &handle);
	else
		glGenTextures(1, &handle);
	return handle;
}
void
_imp_Texture_Finalize(unsigned handle)
{
	if (handle)
		glDeleteTextures(1, &handle);
}

void
_imp_Texture_Bind(unsigned handle, unsigned target, unsigned unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(target, handle);
}
void
_imp_Texture_Unbind(unsigned target, unsigned unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(target, 0);
}

inline void
_aux_WriteTextureParameterDsa(unsigned handle, PixelFormat fmt)
{
	int swizzle[4];
	_aux_GetRearrangedSwizzle(fmt, swizzle);

	glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteriv(handle, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
}
// Warning: Texture must be bound before calling this.
inline void
_aux_WriteTextureParameter(unsigned target, PixelFormat fmt)
{
	int swizzle[4];
	_aux_GetRearrangedSwizzle(fmt, swizzle);

	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteriv(target, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
}

void _imp_Texture_Allocate1D(unsigned handle, unsigned target,
	Media::PixelFormat fmt,
	unsigned width)
{
	if (glTextureStorage1D)
	{
		glTextureStorage1D(handle, 1, GL_RGBA32F, width);
		_aux_WriteTextureParameterDsa(handle, fmt);
	}
	else
	{
		_imp_Texture_Bind(handle, target, 0);
		glTexStorage1D(target, 1, GL_RGBA32F,
			width);
		_aux_WriteTextureParameter(target, fmt);
		_imp_Texture_Unbind(target, 0);
	}
}
void _imp_Texture_Allocate2D(unsigned handle, unsigned target,
	Media::PixelFormat fmt,
	unsigned width, unsigned height)
{
	if (glTextureStorage2D)
	{
		glTextureStorage2D(handle, 1, GL_RGBA32F, width, height);
		_aux_WriteTextureParameterDsa(handle, fmt);
	}
	else
	{
		_imp_Texture_Bind(handle, target, 0);
		glTexStorage2D(target, 1, GL_RGBA32F,
			width, height);
		_aux_WriteTextureParameter(target, fmt);
		_imp_Texture_Unbind(target, 0);
	}
}
void _imp_Texture_Allocate3D(unsigned handle, unsigned target,
	Media::PixelFormat fmt,
	unsigned width, unsigned height, unsigned depth)
{
	if (glTextureStorage3D)
	{
		glTextureStorage3D(handle, 1, GL_RGBA32F,
			width, height, depth);
		_aux_WriteTextureParameterDsa(handle, fmt);
	}
	else
	{
		_imp_Texture_Bind(handle, target, 0);
		glTexStorage3D(target, 1, GL_RGBA32F,
			width, height, depth);
		_aux_WriteTextureParameter(target, fmt);
		_imp_Texture_Unbind(target, 0);
	}
}

void _imp_Texture_Write1D(unsigned handle, unsigned target,
	const void* buf, PixelFormat fmt,
	unsigned width,
	unsigned xOffset)
{
	auto format = _aux_MapPixelFormat(fmt);
	if (glTextureSubImage1D)
		glTextureSubImage1D(handle, 0,
			xOffset,
			width,
			format, GL_UNSIGNED_BYTE, buf);
	else
	{
		_imp_Texture_Bind(handle, target, 0);
		glTexSubImage1D(target, 0,
			xOffset,
			width,
			format, GL_UNSIGNED_BYTE, buf);
		_imp_Texture_Unbind(target, 0);
	}
}
void _imp_Texture_Write2D(unsigned handle, unsigned target,
	const void* buf, PixelFormat fmt,
	unsigned width, unsigned height,
	unsigned xOffset, unsigned yOffset)
{
	auto format = _aux_MapPixelFormat(fmt);
	if (glTextureSubImage2D)
		glTextureSubImage2D(handle, 0,
			xOffset, yOffset,
			width, height,
			format, GL_UNSIGNED_BYTE, buf);
	else
	{
		_imp_Texture_Bind(handle, target, 0);
		glTexSubImage2D(target, 0,
			xOffset, yOffset,
			width, height,
			format, GL_UNSIGNED_BYTE, buf);
		_imp_Texture_Unbind(target, 0);
	}
}
void _imp_Texture_Write3D(unsigned handle, unsigned target,
	const void* buf, PixelFormat fmt,
	unsigned width, unsigned height, unsigned depth,
	unsigned xOffset, unsigned yOffset, unsigned zOffset)
{
	auto format = _aux_MapPixelFormat(fmt);
	if (glTextureSubImage3D)
		glTextureSubImage3D(handle, 0,
			xOffset, yOffset, zOffset,
			width, height, depth,
			format, GL_UNSIGNED_BYTE, buf);
	else
	{
		_imp_Texture_Bind(handle, target, 0);
		glTexSubImage3D(target, 0,
			xOffset, yOffset, zOffset,
			width, height, depth,
			format, GL_UNSIGNED_BYTE, buf);
		_imp_Texture_Unbind(target, 0);
	}
}

void
_imp_Texture_GenerateMipmap(unsigned handle, unsigned target)
{
	if (glGenerateTextureMipmap)
		glGenerateTextureMipmap(handle);
	else
	{
		_imp_Texture_Bind(handle, target, 0);
		glGenerateMipmap(target);
		_imp_Texture_Unbind(target, 0);
	}
}
_L_NS_END_DETAILS

_L_NS_END_GRAPHICS_OPENGL
