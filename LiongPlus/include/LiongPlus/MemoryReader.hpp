// File: MemoryReader.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <stdexcept>
#include "_"

_L_NS_BEG

/*
 * An implementation of stream attached to memory.
 * [MemoryReader] itself will NOT do any clean up work, i.e.
 *   destruction or deallocation. It is banned to be copied or to be
 *   moved in order that the life time of [MemoryReader] is enclosed
 *   by that of the attached memory.
 * This class would help if you have differently typed data in a
 *   continuous buffer. If the buffer contains data of a same type,
 *   the use of raw pointer might be prefered.
 */
class MemoryReader
{
private:
	const Byte* _Pos;
	const Byte* _End;

public:
	MemoryReader() noexcept :
		_Pos(nullptr),
		_End(nullptr)
	{}
	MemoryReader(const void* beg, const void* end) noexcept :
		_Pos(reinterpret_cast<const Byte*>(beg)),
		_End(reinterpret_cast<const Byte*>(end))
	{}
	MemoryReader(const void* beg, size_t size) noexcept :
		_Pos(reinterpret_cast<const Byte*>(beg)),
		_End(reinterpret_cast<const Byte*>(beg) + size)
	{}
	
	template<typename T>
	operator const T*() const noexcept
	{
		return reinterpret_cast<const T*>(_Pos);
	}

	/*
	 * Returns:
	 *   True if the binary read succeeded. Otherwise, false.
	 * Note:
	 *   By default, [T] is not set to [TOut] to prevent unexpected implicit reading behavior.
	 */
	template<typename T, typename TOut>
	bool
		TryRead(TOut& out) noexcept
	{
		if (_Pos + sizeof(T) > _End)
		{
			_Pos = _End;
			return false;
		}
		out = static_cast<TOut>(*reinterpret_cast<const T*>(_Pos));
		_Pos += sizeof(T);
		return true;
	}
	/*
	 * Returns:
	 *   The binary read result.
	 * Exceptions:
	 *   If the read failed, [runtime_error] will be thrown.
	 */
	template<typename T>
	T
		Read()
	{
		T rv;
		if (false == TryRead<T>(rv))
			throw std::runtime_error("Exceeding the end of buffer.");
		return rv;
	}

	template<typename T, typename TIter>
	bool
		TryReadMany(size_t count, TIter out) noexcept
	{
		using std::copy;
		if (_Pos + count * sizeof(T) > _End)
		{
			_Pos = _End;
			return false;
		}
		copy(reinterpret_cast<const T*>(_Pos),
			reinterpret_cast<const T*>(_Pos) + count, out);
		_Pos += count * sizeof(T);
		return true;
	}

	template<typename T, typename TContainer>
	bool
		TryReadManyByIndex(size_t offset, size_t count, TContainer& out) noexcept
	{
		if (_Pos + count * sizeof(T) > _End)
		{
			_Pos = _End;
			return false;
		}
		for (size_t i = 0; i < count; ++i)
		{
			out[offset + i] = *reinterpret_cast<const T*>(_Pos);
			_Pos += sizeof(T);
		}
		return true;
	}

	/*
	 * Skip $count [T]s. If the skip exceeds upper boundary, the result is bound to $_End.
	 */
	template<typename T = uint8_t>
	void
		Skip(size_t count) noexcept
	{
		auto posAfter = _Pos + count * sizeof(T);
		if (posAfter > _End)
			_Pos = _End;
		else
			_Pos = posAfter;
	}
};

_L_NS_END
