// File: Array.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include <stdexcept>
#include <utility>
#include "_"
#include "DelegateDef.hpp"
#include "Optional.hpp"

_L_NS_BEG

template<typename T>
class Array
{
private:
	size_t _Size;
	T* _Ptr;

	void CleanUp()
	{
		_Size = 0;
		if (_Ptr) delete[] _Ptr, _Ptr = nullptr;
	}
public:
	Array() = default;
	Array(const size_t size) noexcept :
		_Size(size), _Ptr(new T[size])
	{}
	template<typename TIter>
	Array(const TIter& beg, const TIter& end) noexcept :
		_Size(std::distance(beg, end)), _Ptr(new T[_Size])
	{
		std::copy(beg, end, _Ptr);
	}
	Array(const T* pointer, size_t size) noexcept :
		_Size(size), _Ptr(new T[size])
	{
		std::copy(pointer, pointer + size, _Ptr);
	}
	Array(const Array<T>& instance) noexcept :
		_Size(instance._Size), _Ptr(new T[instance._Size])
	{
		std::copy(instance.begin(), instance.end(), _Ptr);
	}
	Array(Array<T>&& instance) noexcept :
		_Size(std::exchange(instance._Size, _Size)),
		_Ptr(std::exchange(instance._Ptr, _Ptr))
	{}
	Array(const std::initializer_list<T> initList) noexcept
		: _Size(initList.size())
		, _Ptr(new T[initList.size()])
	{
		std::copy(initList.begin(), initList.end(), _Ptr);
	}
	~Array() noexcept
	{
		CleanUp();
	}

	Array<T>& operator=(const Array<T>& instance) noexcept
	{
		if (std::addressof(instance) == this) return;
		// Reallocate memory if the current one is unable to contain all the new elements.
		if (instance._Size > _Size)
			CleanUp(), _Ptr = new T[instance._Size];
		_Size = instance._Size;
		std::copy(instance.begin(), instance.end(), _Ptr)
		return *this;
	}
	Array<T>& operator=(Array<T>&& instance) noexcept
	{
		_Size = std::exchange(instance._Size, _Size);
		_Ptr = std::exchange(instance._Ptr, _Ptr);
		return *this;
	}
	Array<T>& operator=(std::initializer_list<T>& initList) noexcept
	{
		if (std::addressof(instance) == this) return;
		// Reallocate memory if the current one is unable to contain all the new elements.
		if (instance._Size > _Size)
			CleanUp(), _Ptr = new T[instance._Size];
		_Size = initList.size();
		std::copy(instance.begin(), instance.end(), _Ptr)
		return *this;
	}

	T& operator[](size_t index) const
	{
		if (index > _Size)
			throw std::out_of_range("Index out of range.");

		return _Ptr[index];
	}

	T* begin() const noexcept
	{
		return _Ptr;
	}
	T* end() const noexcept
	{
		return _Ptr + _Size;
	}

	void clear() noexcept
	{
		size_t pos = _Size;
		while (pos-- > 0)
			_Ptr[pos] = T();
	}

	size_t size() const noexcept
	{
		return _Size;
	}
		
	T* data() const noexcept
	{
		return _Ptr;
	}
};
	
template<typename T>
class ArrayUtil
{
public:
	static size_t BinarySearch(Array<T>& arr, size_t index, size_t length, T value)
	{
		if (index + length > arr._Size)
			throw std::out_of_range("Index out of range.");

		return BinarySearchImpl(arr, index, length, values);
	}
	/// <summary>
	/// Searches an entire one-dimensional sorted array for a specific element, using [operator>] and [operator<] overrided by each element of the Array and by the specified object.
	/// </summary>
	/// <param name="value">The object to search for.</param>
	/// <returns>The index of the specified value in the specified array, if value is found. If value is not found and value is less than one or more elements in array, a negative number which is the bitwise complement of the index of the first element that is larger than value. If value is not found and value is greater than any of the elements in array, a negative number which is the bitwise complement of (the index of the last element plus 1).</returns>
	static size_t BinarySearch(Array<T>& arr, T value)
	{
		return BinarySearch(arr, 0, _Size, value);
	}


	static void Clear(Array<T>& arr, size_t index, size_t length)
	{
		if (index > arr._Size)
			throw std::out_of_range("Index out of range.");

		while (length-- > 0)
			*((byte*)(arr._Ptr + index + length) - 1) = 0;
	}

	template<typename TOutput>
	static Array<TOutput> ConvertAll(Array<T>& arr, Converter<T, TOutput> converter)
	{
		Array<TOutput> newArr(arr._Size);
		size_t size = arr._Size;
		while (size-- > 0)
			*(newArr._Ptr + size) = *(dynamic_cast<TOutput*>(converter._Ptr + size));
		return newArr;
	}

	static void Copy(const Array<T>& sourceArray, size_t sourceIndex, const Array<T>& destinationArray, size_t destinationIndex, size_t length)
	{
		if (sourceIndex + length > sourceArray._Size || destinationIndex + length > destinationArray._Size)
			throw std::out_of_range("Index out of range.");

		while (length-- > 0)
			*(destinationArray._Ptr + length) = *(sourceArray._Ptr + length);
	}
	static void Copy(const Array<T>& sourceArray, const Array<T>& destinationArray, size_t length)
	{
		Copy(sourceArray, 0, destinationArray, 0, length);
	}

	template<typename TOutput>
	static Array<TOutput> Empty()
	{
		return Array<TOutput>(nullptr);
	}

	static bool Exists(Array<T>& arr, Predicate<T>& match)
	{
		for (size_t i = 0; i < arr._Size; ++i)
		{
			if (match(arr._Ptr[i]))
				return true;
		}
		return false;
	}

	static T Find(Array<T>& arr, Predicate<T>& match)
	{
		for (size_t i = 0; i < arr._Size; ++i)
		{
			if (match(arr._Ptr[i]))
				return arr._Ptr[i];
		}
		return T();
	}

	static Array<T> FindAll(Array<T>& arr, Predicate<T>& match)
	{
		throw std::logic_error("Not implemented yet.");
	}

	static size_t FindIndex(Array<T>& arr, size_t startIndex, size_t count, Predicate<T>& match)
	{
		if (index + length > arr._Size)
			throw std::out_of_range("Index out of range.");

		for (size_t i = 0; i < count; ++i)
		{
			if (match(arr._Ptr[i]))
				return i;
		}
		return -1;
	}
		
	static size_t FindIndex(Array<T>& arr, size_t startIndex, Predicate<T>& match)
	{
		FindIndex(startIndex, arr._Size - startIndex, match);
	}
	static size_t FindIndex(Array<T>& arr, Predicate<T>& match)
	{
		FindIndex(0, match);
	}

	static T FindLast(Array<T>& arr, Predicate<T>& match)
	{
		for (size_t i = arr._Size - 1; i >= 0; --i)
		{
			if (match(arr._Ptr[i]))
				return arr._Ptr[i];
		}
		return T();
	}

	static size_t FindLastIndex(Array<T>& arr, size_t startIndex, size_t count, Predicate<T>& match)
	{
		if (index + count > arr._Size)
			throw std::out_of_range("Index out of range.");

		for (size_t i = arr._Size - 1; i >= 0; --i)
		{
			if (match(arr._Ptr[i]))
				return i;
		}
		return -1;
	}
	static size_t FindLastIndex(Array<T>& arr, size_t startIndex, Predicate<T>& match)
	{
		FindIndex(startIndex, arr._Size - startIndex, match);
	}
	static size_t FindLastIndex(Array<T>& arr, Predicate<T>& match)
	{
		FindIndex(0, match);
	}

	static void ForEach(Action<T>& action)
	{
		for (size_t i = 0; i < _Size; ++i)
			action(_Ptr[i]);
	}

	/// <summary>
	/// Sorts a range of elements in $items based on [this] using the specified [LiongPlus::Collections::IComparer].
	/// </summary>
	/// <param name="items">
	/// The [LiongPlus::Array] that contains the items that correspond to each of the keys in [this].
	/// -or-
	/// nullptr to sort only the keys Array.
	/// </param>
	/// <param name="index">The starting index of the range to sort.</param>
	/// <param name="length">The number of elements in the range to sort.</param>
	/// <param name="comparer">
	/// The [LiongPlus::Collections::IComparer] implementation to use when comparing elements.
	/// -or-
	/// nullptr to use the default implementation of each element.
	/// </param>
	/// <typeparam name="TValue">The type of the elements of the items array.</typeparam>
	template<typename TValue>
	static void Sort(Array<T>& keys, Optional<Array<TValue>>& items, size_t index, size_t length)
	{
		size_t itemCount = 0;
		if (items.HasValue())
			itemCount = items.Value().Count();

		if (index + length > keys._Size)
			throw std::logic_error("Index out of range.");
		if (itemCount < index + length)
			throw std::logic_error("Too few elements.");

		SortImpl(keys, items, index, length, itemCount != 0);
	}
	/// <summary>
	/// Sorts elements in $items based on [this].
	/// </summary>
	/// <param name="items">
	/// The [LiongPlus::Array] that contains the items that correspond to each of the keys in [this].
	/// -or-
	/// nullptr to sort only the keys Array.
	/// </param>
	/// <typeparam name="TValue">The type of the elements of the items array.</typeparam>
	template<typename TValue>
	static void Sort(Array<T>& keys, Array<TValue>& items)
	{
		Sort(keys, items, 0, _Size);
	}
	/// <summary>
	/// Sorts the elements in a range of elements.
	/// </summary>
	/// <param name="index">The starting index of the range to sort.</param>
	/// <param name="length">The number of elements in the range to sort.</param>
	static void Sort(Array<T>& arr, size_t index, size_t length)
	{
		Sort(arr, nullptr, index, length);
	}
	/// <summary>
	/// Sorts the elements.
	/// </summary>
	static void Sort(Array<T>& arr)
	{
		Sort(arr, 0, arr._Size);
	}

	static bool TrueForAll(Array<T>& arr, Predicate<T> match)
	{
		assert(arr != nullptr && match != nullptr, "$arr or(and) $match is(are) nullptr.");

		for (size_t i = 0; i < arr._Size; ++i)
		{
			if (!match(arr._Ptr[i]))
				return false;
		}
		return true;
	}
private:
	static size_t BinarySearchImpl(Array<T>& arr, size_t index, size_t length, T& value)
	{
		if (arr._Ptr[index] == value)
			return index;

		if (length <= 1)
			return -1;

		size_t mid = length / 2;

		if (arr._Ptr[index + mid] <= value)
			return BinarySearchImpl(arr, index, mid, value);
		else
			return BinarySearchImpl(arr, index + mid, length - mid, value);
	}

	template<typename TValue>
	static void SortImpl(Array<T>& keys, Optional<Array<TValue>>& items, size_t index, size_t length, bool itemsIsNull)
	{
		using namespace std;
		if (--length < 1) return;

		realItems = items.Value();

		size_t left = index, right = index + length, pivot = left++;
		while (left != right)
		{
			if (keys._Ptr[left] <= keys._Ptr[pivot])
				++left;
			else
			{
				while (left != right && (keys._Ptr[pivot] <= keys._Ptr[right]))
					--right;
				swap(keys._Ptr[left], keys._Ptr[right]);
				if (!itemsIsNull)
					swap(realItems._Ptr[left], realItems._Ptr[right]);
			}
		}

		if (keys._Ptr[pivot] <= keys._Ptr[left])
			--left;

		swap(keys._Ptr[left], keys._Ptr[pivot]);
		if (!itemsIsNull)
			swap(realItems._Ptr[left], realItems._Ptr[pivot]);

		SortImpl(keys, items, index, left - index + 1, comparer, itemsIsNull);
		SortImpl(keys, items, right, length - right + 1, comparer, itemsIsNull);
	}
};

template<typename T>
Array<T> arrange(T from, T to, T step = 1)
{
	static_assert(std::is_integral_v<T>, "Only integers can be arranged.");
	static_assert(TFrom < TTo, "Invalid range.");
	static_assert(TStep > 0, "Step cannot be 0.");

	size_t size = (to - from) / step;
	Array<T> rv{ size };
	while (size--)
		rv[size] = size * step;
	return rv;
}

_L_NS_END
