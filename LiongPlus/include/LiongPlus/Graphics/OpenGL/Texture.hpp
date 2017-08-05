// File: Texture.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"
#include "BufferObject.hpp"
#include "../../Media/Image.hpp"

_L_NS_BEG_GRAPHICS_OPENGL

_L_NS_BEG_DETAILS
using namespace LiongPlus::Media;

// Object creation and removal.
extern unsigned
_imp_Texture_Initialize(unsigned target);
extern void
_imp_Texture_Finalize(unsigned handle);

// Texture activation and binding.
extern void
_imp_Texture_Bind(unsigned handle, unsigned target, unsigned unit);
extern void
_imp_Texture_Unbind(unsigned target, unsigned unit);

// Allocate memory for texture.
extern void
_imp_Texture_Allocate1D(unsigned handle, unsigned target,
	Media::PixelFormat fmt,
	unsigned width);
extern void
_imp_Texture_Allocate2D(unsigned handle, unsigned target,
	Media::PixelFormat fmt,
	unsigned width, unsigned height);
extern void
_imp_Texture_Allocate3D(unsigned handle, unsigned target,
	Media::PixelFormat fmt,
	unsigned width, unsigned height, unsigned depth);

// Write data to texture. This must be done through pixel unpack buffer.
extern void
_imp_Texture_Write1D(unsigned handle, unsigned target,
	const void* buf, PixelFormat fmt,
	unsigned width,
	unsigned xOffset);
extern void
_imp_Texture_Write2D(unsigned handle, unsigned target,
	const void* buf, PixelFormat fmt,
	unsigned width, unsigned height,
	unsigned xOffset, unsigned yOffset);
extern void
_imp_Texture_Write3D(unsigned handle, unsigned target,
	const void* buf, PixelFormat fmt,
	unsigned width, unsigned height, unsigned depth,
	unsigned xOffset, unsigned yOffset, unsigned zOffset);

// Mipmap generation.
extern void
_imp_Texture_GenerateMipmap(unsigned handle, unsigned target);

_L_NS_END_DETAILS

enum class TextureType
{
	Texture1D = 0x0DE0,
	Texture2D = 0x0DE1,
	Texture2DArray = 0x8C1A,
};

template<TextureType TType>
class Texture
{
private:
	unsigned _Handle, _Target;
	unsigned _Width, _Height, _Depth;

public:
	class BindingGuard
	{
	private:
		unsigned _Target;
		unsigned _Unit;

	public:
		BindingGuard(unsigned target, unsigned unit, unsigned handle) :
			_Target(target),
			_Unit(unit)
		{
			details::_imp_Texture_Bind(handle, target, unit);
		}
		BindingGuard(BindingGuard&& inst) :
			_Target(std::exchange(inst._Target, 0)),
			_Unit(std::exchange(inst._Unit, 0))
		{}
		~BindingGuard()
		{
			if (_Target)
				details::_imp_Texture_Unbind(_Target, _Unit);
		}
	};

	Texture() :
		_Target(unsigned(TType)),
		_Handle(details::_imp_Texture_Initialize(unsigned(TType)))
	{}
	Texture(Texture&&) :
		_Handle(std::exchange(inst._Handle, 0)),
		_Width(std::exchange(inst._Width, 0)),
		_Height(std::exchange(inst._Height, 0)),
		_Depth(std::exchange(inst._Depth, 0))
	{}
	~Texture()
	{
		details::_imp_Texture_Finalize(_Handle);
	}

	Texture& operator=(Texture&&)
	{
		details::_imp_Texture_Finalize(_Handle);
		_Handle = std::exchange(inst._Handle, 0);
		_Width = std::exchange(inst._Width, 0);
		_Height = std::exchange(inst._Height, 0);
		_Depth = std::exchange(inst._Depth, 0);
		return *this;
	}
	operator unsigned() const
	{
		return _Handle;
	}
	
	BindingGuard Bind(unsigned unit)
	{
		return { _Target, unit, _Handle };
	}

	// Texture1D only.
	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture1D == UType>
		Allocate(Media::PixelFormat fmt,
			unsigned width)
	{
		details::_imp_Texture_Allocate1D(_Handle, _Target, fmt,
			_Width = width);
	}

	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture1D == UType>
		Write(const void* buf,
			Media::PixelFormat fmt)
	{
		Write(buf, fmt, _Width);
	}
	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture1D == UType>
		Write(const void* buf,
			Media::PixelFormat fmt,
			unsigned width)
	{
		Write(buf, fmt, width, 0);
	}
	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture1D == UType>
		Write(const void* buf,
			Media::PixelFormat fmt,
			unsigned width,
			unsigned xOffset)
	{
		details::_imp_Texture_Write1D(_Handle, _Target, buf, fmt,
			width,
			xOffset);
	}
	// Texture2D only.
	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture2D == UType>
		Allocate(Media::PixelFormat fmt,
			unsigned width, unsigned height)
	{
		details::_imp_Texture_Allocate2D(_Handle, _Target, fmt,
			_Width = width, _Height = height);
	}
	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture2D == UType>
		Write(const void* buf,
			Media::PixelFormat fmt)
	{
		Write(buf, fmt, _Width, _Height);
	}
	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture2D == UType>
		Write(const void* buf,
			Media::PixelFormat fmt,
			unsigned width, unsigned height)
	{
		Write(buf, fmt, width, height, 0, 0);
	}
	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture2D == UType>
		Write(const void* buf,
			Media::PixelFormat fmt,
			unsigned width, unsigned height,
			unsigned xOffset, unsigned yOffset)
	{
		details::_imp_Texture_Write2D(_Handle, _Target, buf, fmt,
			width, height,
			xOffset, yOffset);
	}
	// Texture2DArray (3D texture) only.
	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture2DArray == UType>
		Allocate(Media::PixelFormat fmt,
			unsigned width, unsigned height, unsigned depth)
	{
		details::_imp_Texture_Allocate3D(_Handle, _Target, fmt,
			_Width = width, _Height = height, _Depth = depth);
	}
	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture2DArray == UType>
		Write(const void* buf,
			Media::PixelFormat fmt)
	{
		Write(buf, fmt, _Width, _Height, _Depth);
	}
	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture2DArray == UType>
		Write(const void* buf,
			Media::PixelFormat fmt,
			unsigned width, unsigned height, unsigned depth)
	{
		Write(buf, fmt, width, height, depth, 0, 0, 0);
	}
	template<TextureType UType = TType>
	std::enable_if_t<TextureType::Texture2DArray == UType>
		Write(const void* buf,
			Media::PixelFormat fmt,
			unsigned width, unsigned height, unsigned depth,
			unsigned xOffset, unsigned yOffset, unsigned zOffset)
	{
		details::_imp_Texture_Write3D(_Handle, _Target, buf, fmt,
			width, height, depth,
			xOffset, yOffset, zOffset);
	}

	void GenerateMipmap()
	{
		details::_imp_Texture_GenerateMipmap(_Handle, _Target);
	}
};

using Texture1D = Texture<TextureType::Texture1D>;
using Texture2D = Texture<TextureType::Texture2D>;
using Texture2DArray = Texture<TextureType::Texture2DArray>;

_L_NS_END_GRAPHICS_OPENGL
