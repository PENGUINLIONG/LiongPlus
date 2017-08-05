// File: BufferObject.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "_"

_L_NS_BEG_GRAPHICS_OPENGL

_L_NS_BEG_DETAILS
// Object creation/removal.
extern unsigned
_imp_BufferObject_Initialize();
extern void
_imp_BufferObject_Finalize(unsigned handle);

// General binding.
extern void
_imp_BufferObject_Bind(unsigned handle, unsigned target);
extern void
_imp_BufferObject_Unbind(unsigned target);

// These two functions only work for indexed targets.
extern void
_imp_BufferObject_BindIndex(unsigned handle, unsigned target, unsigned index, unsigned offset, unsigned size);
extern void
_imp_BufferObject_UnbindIndex(unsigned target, unsigned index);

// Server memory allocation.
extern void
_imp_BufferObject_Allocate(unsigned handle, unsigned target, unsigned size, bool mapThanSub, bool readable);
extern void
_imp_BufferObject_AllocateResizable(unsigned handle, unsigned target, unsigned size, unsigned accessCode);

// Memory mapping.
extern void*
_imp_BufferObject_Map(unsigned handle, unsigned target, unsigned offset, unsigned size, bool readable, bool mapThanSub);
extern void
_imp_BufferObject_Unmap(unsigned handle, unsigned target);
extern void
_imp_BufferObject_Flush(unsigned handle, unsigned target, unsigned offset, unsigned size);

// Writing to memory.
extern void
_imp_BufferObject_Write(unsigned handle, unsigned target, unsigned offset, unsigned size, const void* data);

// This two functions only work for pixel buffer targets.
extern void
_imp_BufferObject_SetPixelProperty(unsigned prop, int val);

_L_NS_END_DETAILS

enum class BindingTarget
{
	ArrayBuffer = 0x8892,
	AtomicCounterBuffer = 0x92C0,
	DrawIndirectBuffer = 0x8F3F,
	ElementArrayBuffer = 0x8893,
	PixelPackBuffer = 0x88EB,
	PixelUnpackBuffer = 0x88EC,
	ShaderStorageBuffer = 0x90D2,
	TransformFeedbackBuffer = 0x8C8E,
	UniformBuffer = 0x8A11,
};
enum class BufferAccessPattern
{
	// Use once and abandoned. e.g. Temporary texture storage before
	//   TexBuffer.
	OneOff,
	// Buffer frequently abandoned. e.g. Dynamically generated mesh.
	FrequentResize,
	// Frequent read but seldom write. e.g. Texture that doesn't
	//   change in content.
	FrequentRead,
	// Frequent read and write without resizing. e.g. Mesh rendering.
	FrequentReadWrite,
};

template<BindingTarget TBind>
class BufferObject
{
private:
	unsigned _Size;
	unsigned _Target;
	unsigned _Handle;
	void* _PersistentlyMappedPtr;
	bool _MapRead;

public:
	class BindingGuard
	{
	private: unsigned _Target;
	public:
		BindingGuard(unsigned target, unsigned handle)
			: _Target(target)
		{
			details::_imp_BufferObject_Bind(handle, target);
		}
		BindingGuard(BindingGuard&& inst)
			: _Target(std::exchange(inst._Target, 0))
		{}
		~BindingGuard()
		{
			if (_Target)
				details::_imp_BufferObject_Unbind(_Target);
		}
	};
	class IndexBindingGuard
	{
	private: unsigned _Target, _Index;
	public:
		IndexBindingGuard(unsigned handle, unsigned target, unsigned index, unsigned offset, unsigned size) :
			_Target(target),
			_Index(index)
		{
			details::_imp_BufferObject_BindIndex(handle, target, index, offset, size);
		}
		IndexBindingGuard(IndexBindingGuard&& inst) :
			_Target(std::exchange(inst._Target, 0)),
			_Index(std::exchange(inst._Index, 0))
		{
		}
		~IndexBindingGuard()
		{
			details::_imp_BufferObject_UnbindIndex(_Target, _Index);
		}
	};
	class MappingGuard
	{
	private:
		void* _Ptr;
		unsigned _Handle;
		unsigned _Target;
		unsigned _Offset;
		unsigned _Size;
		bool _NeedUnmap;

	public:
		MappingGuard(void* ptr, unsigned handle, unsigned target, unsigned offset, unsigned size, bool needUnmap) :
			_Ptr(ptr),
			_Handle(handle),
			_Target(target),
			_Offset(offset),
			_Size(size),
			_NeedUnmap(needUnmap)
		{}
		MappingGuard(MappingGuard&& inst) :
			_Ptr(std::exchange(inst._Ptr, nullptr)),
			_Handle(std::exchange(inst._Handle, 0)),
			_Target(std::exchange(inst._Target, 0)),
			_Offset(std::exchange(inst._Offset, 0)),
			_Size(std::exchange(inst._Size, 0)),
			_NeedUnmap(std::exchange(inst._NeedUnmap, false))
		{}
		~MappingGuard()
		{
			details::_imp_BufferObject_Flush(_Handle, _Target, _Offset, _Size);
			if (_NeedUnmap)
				details::_imp_BufferObject_Unmap(_Handle, _Target);
		}

		template<typename U>
		operator U*() const
		{
			return reinterpret_cast<U*>((char*)(_Ptr) + _Offset);
		}
	};

	BufferObject() :
		_Size(),
		_Target((unsigned)TBind),
		_Handle(details::_imp_BufferObject_Initialize()),
		_PersistentlyMappedPtr(nullptr),
		_MapRead(false)
	{}
	BufferObject(BufferObject&& inst) :
		_Size(std::exchange(inst._Size, 0)),
		_Target(std::exchange(inst._Target, 0)),
		_Handle(std::exchange(inst._Handle, 0)),
		_PersistentlyMappedPtr(std::exchange(inst._PersistentlyMappedPtr, nullptr)),
		_MapRead(std::exchange(inst._MapRead, false))
	{}
	~BufferObject()
	{
		if (_PersistentlyMappedPtr)
			details::_imp_BufferObject_Unmap(_Handle, _Target);
		details::_imp_BufferObject_Finalize(_Handle);
	}

	BufferObject& operator=(BufferObject&& inst)
	{
		details::_imp_BufferObject_Finalize(_Handle);
		_Size = std::exchange(inst._Size, 0);
		_Target = std::exchange(inst._Target, 0);
		_Handle = std::exchange(inst._Handle, 0);
		_PersistentlyMappedPtr = std::exchange(inst._PersistentlyMappedPtr, nullptr);
		_MapRead = std::exchange(inst._MapRead, false);
		return *this;
	}

	operator unsigned() const { return _Handle; }
	void Allocate(unsigned size, bool mapThanSub = true,
		bool readable = false)
	{
		_Size = size;
		_MapRead = readable;
		details::_imp_BufferObject_Allocate(
			_Handle, _Target, size, mapThanSub, readable);
		if (mapThanSub)
			_PersistentlyMappedPtr = details::_imp_BufferObject_Map(
				_Handle, _Target, 0, size, readable, true);
	}
	void AllocateResizable(unsigned size, BufferAccessPattern access)
	{
		_Size = size;
		details::_imp_BufferObject_AllocateResizable(_Handle, _Target, size, unsigned(access));
	}

	void Write(const void* data)
	{
		Write(data, 0, _Size);
	}
	void Write(const void* data, unsigned targetOffset, unsigned size)
	{
		details::_imp_BufferObject_Write(_Handle, _Target, targetOffset, size, data);
	}

	BindingGuard Bind() const
	{
		return { _Target, _Handle };
	}

	template<BindingTarget UBind = TBind,
		typename U = std::enable_if_t<
		BindingTarget::AtomicCounterBuffer == UBind ||
		BindingTarget::ShaderStorageBuffer == UBind ||
		BindingTarget::TransformFeedbackBuffer == UBind ||
		BindingTarget::UniformBuffer == UBind,
		unsigned>>
		IndexBindingGuard BindIndex(U index) const
	{
		return BindIndex(index, 0, _Size);
	}
	template<BindingTarget UBind = TBind>
	std::enable_if_t<
		BindingTarget::AtomicCounterBuffer == UBind ||
		BindingTarget::ShaderStorageBuffer == UBind ||
		BindingTarget::TransformFeedbackBuffer == UBind ||
		BindingTarget::UniformBuffer == UBind, IndexBindingGuard>
		BindIndex(unsigned index, unsigned offset, unsigned size) const
	{
		return { _Handle, _Target, index, offset, size };
	}

	MappingGuard Map()
	{
		return Map(0, _Size);
	}
	MappingGuard Map(unsigned offset, unsigned size)
	{
		if (_PersistentlyMappedPtr)
			return { _PersistentlyMappedPtr, _Handle, _Target, offset, size, false };
		else
		{
			return {
				details::_imp_BufferObject_Map(
					_Handle, _Target, offset, size, _MapRead, false),
				_Handle, _Target, offset, size, true
			};
		}
	}
};

// Aliases.
using ArrayBuffer = BufferObject<BindingTarget::ArrayBuffer>;
using AtomicCounterBuffer =
BufferObject<BindingTarget::AtomicCounterBuffer>;
using DrawIndirectBuffer =
BufferObject<BindingTarget::DrawIndirectBuffer>;
using ElementArrayBuffer =
BufferObject<BindingTarget::ElementArrayBuffer>;
using PixelPackBuffer = BufferObject<BindingTarget::PixelPackBuffer>;
using PixelUnpackBuffer =
BufferObject<BindingTarget::PixelUnpackBuffer>;
using ShaderStorageBuffer =
BufferObject<BindingTarget::ShaderStorageBuffer>;
using TransformFeedbackBuffer =
BufferObject<BindingTarget::TransformFeedbackBuffer>;
using UniformBuffer = BufferObject<BindingTarget::UniformBuffer>;

_L_NS_END_GRAPHICS_OPENGL
