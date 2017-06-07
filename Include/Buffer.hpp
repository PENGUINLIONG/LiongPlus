// File: Buffer.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <string>
#include <utility>
#include "_"

_L_NS_BEG

using std::swap;

class Buffer
{
	friend void swap(Buffer& x, Buffer& y)
	{
		swap(x._Field, y._Field);
		swap(x._Length, y._Length);
	}
private:
	Byte* _Field;
	size_t _Length;
public:
	Buffer();
	Buffer(const Buffer&);
	Buffer(Buffer&&);
	Buffer(size_t length);
	Buffer(const std::string& str);
	~Buffer();

	Buffer& operator=(const Buffer&);
	Buffer& operator=(Buffer&&);
	Byte& operator[](size_t index);
	const Byte& operator[](size_t index) const;
		
	void CopyTo(void* dst, size_t index, size_t count) const;
	Buffer Clone() const;
	Byte* Field();
	const Byte* Field() const;
	size_t Length() const;
	void Wipe();
	void Wipe(size_t to);
};

_L_NS_END

