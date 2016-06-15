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
<<<<<<< HEAD
		Buffer();
		Buffer(const Buffer&);
		Buffer(Buffer&&);
		Buffer(size_t length);
		Buffer(const char* str);
		~Buffer();

		Buffer& operator=(const Buffer&);
		Buffer& operator=(Buffer&&);
		Byte& operator[](size_t index);
		
		void CopyTo(void* dst, size_t index, size_t count) const;
		Buffer Clone() const;
		Byte* Field();
		const Byte* Field() const;
		size_t Length() const;
		void Wipe();
=======
		template<typename T>
		static void BlockCopy(Array<T>& src, long srcOffset, Array<T>& dst, long dstOffset, long count);

		template<typename T>
		static long ByteLength(Array<T>& array);

		static void Memcpy(Byte* dst, const Byte* src, long len);

		static void Memset(void* dst, const Byte value, long size);

		static void Memxchg(Byte* dst, Byte* src, long size);
		
		template<typename T>
		static void SetByte(Array<T>& array, long index, Byte value);

		/// <summary>
		/// Get the length of a C-String.
		/// </summary>
		/// <param name="c_str">The C-String to be checked.</param>
		/// <returns>Length of C-String $c_str.</returns>
		/// <remarks>
		/// A surprising hack from .NET Framework source code.
		/// Check this out: http://referencesource.microsoft.com/mscorlib/system/string.cs.html#a1560789e961359b
		/// </remarks>
		static long Wcslen(const _L_Char* c_str);

		/// <summary>
		/// Better wcscpy() with LiongPlus::Buffer class.
		/// </summary>
		/// <param name="dmem">Destination memory.</param>
		/// <param name="smem">Source memory.</param>
		/// <param name="charCount">Count of chars to be copied.</param>
		static void Wcscpy(_L_Char* dmem, const _L_Char* smem, long charCount);
>>>>>>> master
	};
}
