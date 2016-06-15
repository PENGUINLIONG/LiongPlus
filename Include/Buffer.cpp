// File: Buffer.cpp
// Author: Rendong Liang (Liong)

#include "Buffer.hpp"

namespace LiongPlus
{
<<<<<<< HEAD
	using std::swap;

	Buffer::Buffer()
		: _Field(nullptr)
		, _Length(0)
=======
	template <typename T>
	void Buffer::BlockCopy(Array<T>& src, long srcOffset, Array<T>& dst, long dstOffset, long count)
>>>>>>> master
	{
	}
<<<<<<< HEAD
	Buffer::Buffer(size_t length)
		: _Field(new Byte[length])
		, _Length(length)
=======

	template <typename T>
	long Buffer::ByteLength(Array<T>& array)
>>>>>>> master
	{
	}
<<<<<<< HEAD
	Buffer::Buffer(const Buffer& instance)
		: _Field(nullptr)
		, _Length(0)
=======

	template <typename T>
	void Buffer::SetByte(Array<T>& array, long index, Byte value)
>>>>>>> master
	{
		*this = instance.Clone();
	}
<<<<<<< HEAD
	Buffer::Buffer(Buffer&& instance)
		: Buffer()
=======

	void Buffer::Memcpy(Byte* dst, const Byte* src, long length)
>>>>>>> master
	{
		swap(_Field, instance._Field);
		swap(_Length, instance._Length);
	}
<<<<<<< HEAD
	Buffer::Buffer(const char* str)
		: _Length(strlen(str) + 1)
		, _Field(new char[_Length])
=======

	void Buffer::Memset(void * dst, Byte value, long size)
>>>>>>> master
	{
		strcpy(_Field, str);
	}
	Buffer::~Buffer()
	{
		if (_Field != nullptr)
		{
			delete[] _Field;
			_Field = nullptr;
		}
		_Length = 0;
	}


<<<<<<< HEAD
	Buffer& Buffer::operator=(const Buffer& instance)
=======
	void Buffer::Memxchg(Byte* a, Byte* b, long size)
	{
		long blockCopyTimes = size / sizeof(long);
		long remains = size % sizeof(long);

		while (blockCopyTimes-- > 0)
		{
			Swap(((long*)a)[blockCopyTimes], ((long*)b)[blockCopyTimes]);
		}
		switch (remains)
		{
			case 1:
				Swap(((Byte*)a)[size - 1], ((Byte*)b)[size - 1]);
				break;
			case 2:
				Swap(((short*)a)[size - 1], ((short*)b)[size - 1]);
				break;
			case 3:
				Swap(((Byte*)a)[size - 1], ((Byte*)b)[size - 1]);
				Swap(((short*)(a - 1))[size - 1], ((short*)(b - 1))[size - 1]);
				break;
#ifdef _L_X64
			case 4:
				Swap(((int*)a)[size - 1], ((int*)b)[size - 1]);
				break;
			case 5:
				Swap(((Byte*)a)[size - 1], ((Byte*)b)[size - 1]);
				Swap(((int*)(a - 1))[size - 1], ((int*)(b - 1))[size - 1]);
				break;
			case 6:
				Swap(((short*)a)[size - 1], ((short*)b)[size - 1]);
				Swap(((int*)(a - 2))[size - 1], ((int*)(b - 2))[size - 1]);
				break;
			case 7:
				Swap(((Byte*)a)[size - 1], ((Byte*)b)[size - 1]);
				Swap(((short*)(a - 1))[size - 1], ((short*)(b - 1))[size - 1]);
				Swap(((int*)(a - 3))[size - 1], ((int*)(b - 3))[size - 1]);
				break;
#endif
			default:
				break;
		}
	}

	long Buffer::Wcslen(const _L_Char * c_str)
>>>>>>> master
	{
		*this = instance.Clone();
		return *this;
	}
	Buffer& Buffer::operator=(Buffer&& instance)
	{
		swap(_Field, instance._Field);
		swap(_Length, instance._Length);
		return *this;
	}
	Byte& Buffer::operator[](size_t index)
	{
		if (index >= 0 && index < _Length)
			return _Field[index];
		else throw std::runtime_error("$index is out of range.");
	}

	void Buffer::CopyTo(void* dst, size_t index, size_t count) const
	{
		std::memcpy(dst, _Field + index, count);
	}

<<<<<<< HEAD
	Buffer Buffer::Clone() const
	{
		Buffer buffer(_Length);
		std::memcpy(buffer._Field, _Field, _Length);
=======
		// First make sure our pointer is aligned on a dword boundary
		while (((unsigned long)end & 3) != 0 && *end != 0)
			end++;
		if (*end != 0) {
			// The loop condition below works because if "end[0] & end[1]" is non-zero, that means
			// neither operand can have been zero. If is zero, we have to look at the operands individually,
			// but we hope this going to fairly rare.
>>>>>>> master

		return buffer;
	}

	size_t Buffer::Length() const
	{
		return _Length;
	}

<<<<<<< HEAD
	Byte* Buffer::Field()
	{
		return _Field;
	}
	const Byte* Buffer::Field() const
	{
		return _Field;
	}

	void Buffer::Wipe()
=======
		return (long)(end - c_str);
	}

	void Buffer::Wcscpy(_L_Char* dmem, const _L_Char* smem, long charCount)
>>>>>>> master
	{
		memset(_Field, 0, _Length);
	}
}