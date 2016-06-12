// File: Buffer.hpp
// Author: Rendong Liang (Liong)

#pragma once
#include "Fundamental.hpp"

namespace LiongPlus
{
	class Buffer
	{
		friend void swap(Buffer& x, Buffer& y)
		{
			using std::swap;
			swap(x._Field, y._Field);
			swap(x._Length, y._Length);
		}
	private:
		Byte* _Field;
		size_t _Length;
	public:
		Buffer();
		Buffer(const Buffer&) = delete;
		Buffer(Buffer&&);
		Buffer(size_t length);
		Buffer(const char* str);
		~Buffer();

		Buffer& operator=(Buffer&&);
		Byte& operator[](size_t index);
		
		size_t Length() const;
		Byte* Field();
		const Byte* Field() const;
		void Wipe();
	};
}
