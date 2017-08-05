// File: MemoryWriter.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <stdexcept>
#include "_"

_L_NS_BEG

/*
 * See [MemoryReader].
 */
class MemoryWriter
{
private:
	Byte* _Pos;
	const Byte* _End;

public:
	MemoryWriter() noexcept :
		_Pos(nullptr),
		_End(nullptr)
	{}
	MemoryWriter(void* beg, const void* end) noexcept :
		_Pos(reinterpret_cast<Byte*>(beg)),
		_End(reinterpret_cast<const Byte*>(end))
	{}
	MemoryWriter(void* beg, size_t size) noexcept :
		_Pos(reinterpret_cast<Byte*>(beg)),
		_End(reinterpret_cast<const Byte*>(beg) + size)
	{}
	
	template<typename T>
	operator T*() const noexcept
	{
		return reinterpret_cast<T*>(_Pos);
	}

	/*
	 * Returns:
	 *   True if the binary read succeeded. Otherwise, false.
	 * Note:
	 *   By default, [T] is not set to [TOut] to prevent unexpected implicit reading behavior.
	 */
	template<typename T, typename TIn>
	bool
		TryWrite(const TIn& in) noexcept
	{
		if (_Pos + sizeof(T) > _End)
		{
			_Pos = const_cast<Byte*>(_End);
			return false;
		}
		*reinterpret_cast<T*>(_Pos) = (T)in;
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
	void
		Write(const T& in)
	{
		T rv;
		if (false == TryWrite<T>(in))
			throw std::runtime_error("Exceeding the end of buffer.");
		return rv;
	}

	template<typename T, typename TIter>
	bool
		TryWriteMany(size_t count, TIter in) noexcept
	{
		using std::copy;
		if (_Pos + count * sizeof(T) > _End)
		{
			_Pos = const_cast<Byte*>(_End);
			return false;
		}
		copy(in, in + count, reinterpret_cast<T*>(_Pos));
		_Pos += count * sizeof(T);
		return true;
	}

	template<typename T, typename TContainer>
	bool
		TryWriteManyByIndex(
		size_t offset, size_t count, const TContainer& in) noexcept
	{
		if (_Pos + count * sizeof(T) > _End)
		{
			_Pos = const_cast<Byte*>(_End);
			return false;
		}
		for (size_t i = 0; i < count; ++i)
		{
			*reinterpret_cast<T*>(_Pos) = in[offset + i];
			_Pos += sizeof(T);
		}
		return true;
	}

	template<typename T = uint8_t>
	void
		Skip(size_t count) noexcept
	{
		auto posAfter = _Pos + count * sizeof(T);
		_Pos = (posAfter > _End) ? const_cast<Byte*>(_End) : posAfter;
	}
};

_L_NS_END
