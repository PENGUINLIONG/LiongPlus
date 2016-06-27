// File: Buffer.cpp
// Author: Rendong Liang (Liong)

#include "Buffer.hpp"

namespace LiongPlus
{
	using std::swap;

	Buffer::Buffer()
		: _Field(nullptr)
		, _Length(0)
	{
	}
	Buffer::Buffer(size_t length)
		: _Field(new Byte[length])
		, _Length(length)
	{
	}
	Buffer::Buffer(const Buffer& instance)
		: Buffer()
	{
		_Length = instance._Length;
		if (_Length > 0)
		{
			_Field = new Byte[_Length];
			instance.CopyTo(_Field, 0, _Length);
		}
	}
	Buffer::Buffer(Buffer&& instance)
		: Buffer()
	{
		swap(_Field, instance._Field);
		swap(_Length, instance._Length);
	}
	Buffer::Buffer(const char* str)
		: _Length(strlen(str) + 1)
		, _Field(new char[_Length])
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


	Buffer& Buffer::operator=(const Buffer& instance)
	{
		_Length = instance._Length;
		if (_Length > 0)
		{
			_Field = new Byte[instance._Length];
			instance.CopyTo(_Field, 0, _Length);
		}
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

	Buffer Buffer::Clone() const
	{
		Buffer buffer(_Length);
		if (_Length > 0)
			std::memcpy(buffer._Field, _Field, _Length);

		return buffer;
	}

	size_t Buffer::Length() const
	{
		return _Length;
	}

	Byte* Buffer::Field()
	{
		return _Field;
	}
	const Byte* Buffer::Field() const
	{
		return _Field;
	}

	void Buffer::Wipe()
	{
		memset(_Field, 0, _Length);
	}
}