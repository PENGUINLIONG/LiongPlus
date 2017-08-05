// File: Buffer.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <utility>
#include "_"
#include "Array.hpp"

_L_NS_BEG

class Buffer
{
private:
	Array<Byte> _Buf;

public:
	Buffer() : _Buf() {}
	Buffer(const Buffer& inst) : _Buf(inst._Buf) {}
	Buffer(Buffer&& inst) :
		_Buf(std::exchange(inst._Buf, Array<Byte>{})) {}
	Buffer(size_t length) : _Buf(length) {}
	~Buffer() {}

	Buffer& operator=(const Buffer& inst)
	{ _Buf = inst._Buf; return *this; }
	Buffer& operator=(Buffer&& inst)
	{ _Buf = std::exchange(inst._Buf, Array<Byte>{}); return *this; }
	Byte& operator[](size_t index) { return _Buf[index]; }
	const Byte& operator[](size_t index) const { return _Buf[index]; }
		
	void CopyTo(void* dst, size_t index, size_t count) const
	{ std::memcpy(dst, _Buf.data() + index, count); }
	Byte* Field() { return _Buf.data(); }
	const Byte* Field() const { return _Buf.data(); }
	size_t Length() const { return _Buf.size(); }
	void Wipe(int value = 0) { std::memset(_Buf.data(), 0, _Buf.size()); }
	void Wipe(int value, size_t size)
	{ std::memset(_Buf.data(), 0, size); }
};

_L_NS_END

