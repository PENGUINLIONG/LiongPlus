// File: Buffer.cpp
// Author: Rendong Liang (Liong)
#include "Buffer.hpp"

namespace LiongPlus
{
	template <typename T>
	void Buffer::BlockCopy(Array<T>& src, long srcOffset, Array<T>& dst, long dstOffset, long count)
	{
		Memcpy((Byte*)(dst.GetNativePointer() + dstOffset), (const Byte*)(src.GetNativePointer() + srcOffset), count * sizeof(T));
	}

	template <typename T>
	long Buffer::ByteLength(Array<T>& array)
	{
		return array.Length() * sizeof(T);
	}

	template <typename T>
	void Buffer::SetByte(Array<T>& array, long index, Byte value)
	{
		*((Byte*)array.GetNativePointer() + index) = value;
	}

	void Buffer::Memcpy(Byte* dst, const Byte* src, long length)
	{
		memcpy(dst, src, length);
	}

	void Buffer::Memset(void * dst, Byte value, long size)
	{
		memset(dst, value, size);
	}

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
	{
		_L_Char* end = (_L_Char*)c_str;

		// The following code is (somewhat surprisingly!) significantly faster than a naive loop,
		// at least on x86 and the current jit.

		// First make sure our pointer is aligned on a dword boundary
		while (((unsigned long)end & 3) != 0 && *end != 0)
			end++;
		if (*end != 0) {
			// The loop condition below works because if "end[0] & end[1]" is non-zero, that means
			// neither operand can have been zero. If is zero, we have to look at the operands individually,
			// but we hope this going to fairly rare.

			// In general, it would be incorrect to access end[1] if we haven't made sure
			// end[0] is non-zero. However, we know the ptr has been aligned by the loop above
			// so end[0] and end[1] must be in the same page, so they're either both accessible, or both not.

			while ((end[0] & end[1]) != 0 || (end[0] != 0 && end[1] != 0)) {
				end += 2;
			}
		}
		// finish up with the naive loop
		while (*(end++) != 0)
			;

		return (long)(end - c_str);
	}

	void Buffer::Wcscpy(_L_Char* dmem, const _L_Char* smem, long charCount)
	{
		Buffer::Memcpy((Byte*)dmem, (const Byte*)smem, charCount * sizeof(_L_Char));
	}
}