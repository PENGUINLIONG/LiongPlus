// File: Array.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Array
#define _L_Array
#include "Fundamental.hpp"
#include "Collections\ContinuousMemoryEnumerator.hpp"
#include "Collections\ICollection.hpp"
#include "Collections\IComparer.hpp"
#include "Collections\IEnumerable.hpp"
#include "Collections\IList.hpp"
#include "Delegate.hpp"
#include "Exception.hpp"
#include "IO/IBuffer.hpp"
#include "Ptr.hpp"
	
using namespace LiongPlus::Collections;
using namespace LiongPlus::IO;

namespace LiongPlus
{
	/// <summary>
	/// Provides methods for creating, manipulating, searching, and sorting arrays.
	/// </summary>
	/// <typeparam name="T">Type of object.</typeparam>
	/// <remarks>This class is not recommended. Non-template array seems not appropriate for C++.</remarks>
	template<typename T>
	class Array
		: public Object
		, public IBuffer
		, public ICollection<T>
		, public IEnumerable<T>
		, public IList<T>
	{
	public:
		Array()
			: _Size(0)
			, _Ptr(nullptr)
			, _Counter(nullptr)
		{
		}
		Array(const long size)
			: _Size(size)
			, _Ptr(new T[size])
			, _Counter(new ReferenceCounter())
		{
			_Counter->Inc();
		}
		Array(const T* pointer, long size)
			: _Size(size)
			, _Ptr(new T[size])
			, _Counter(new ReferenceCounter())
		{
			while (size-- > 0)
				*(_Ptr + size) = *(pointer + size);
			_Counter->Inc();
		}
		Array(const Array<T>& instance)
			: _Size(instance._Size)
			, _Ptr(new T[instance._Size])
			, _Counter(new ReferenceCounter())
		{
			Copy(instance, *this, 0);
			if (_Counter != nullptr)
				_Counter->Inc();
		}
		Array(Array<T>&& instance)
		{
			Swap(_Size, instance._Size);
			Swap(_Ptr, instance._Ptr);
			Swap(_Counter, instance._Counter);
		}
		Array(const std::initializer_list<T> initList)
			: _Size(initList.size())
			, _Ptr(new T[initList.size()])
			, _Counter(new ReferenceCounter())
		{
			long i = 0;
			for (auto t : initList)
				_Ptr[i++] = t;
		}
		~Array()
		{
			CleanUp();
		}

		Array<T>& operator=(const Array<T>& instance)
		{
			CleanUp();
			_Size = instance._Size;
			_Ptr = instance._Ptr;
			_Counter = instance._Counter;
			if (_Counter != nullptr)
				_Counter->Inc();
			return *this;
		}
		Array<T>& operator=(Array<T>&& instance)
		{
			Swap(_Size, instance._Size);
			Swap(_Ptr, instance._Ptr);
			Swap(_Counter, instance._Counter);
			return *this;
		}
		Array<T>& operator=(std::initializer_list<T>& initList)
		{
			CleanUp();
			_Size = initList.size();
			T* field = new T[_Size];
			long i = 0;
			for (auto t : initList)
				field[i++] = t;
			_Ptr = field;
			return *this;
		}

		T& operator[](long index) const
		{
			assert(index >= 0 && index < _Size, "index");

			return _Ptr[index];
		}

		/// <summary>
		/// Take the advantage of C++11 range-based loop. Never call this method, please call GetEnumerator for [LiongPlus::Collections::IEnumerator].
		/// </summary>
		/// <returns>Parsed [LiongPlus::Collections::IEnumerator] which at the end of the collection.</returns>
		EnumeratorParser<T> begin()
		{
			return new TEnumerator(_Ptr, _Size, 0);
		}
		/// <summary>
		/// Take the advantage of C++11 range-based loop. Never call this method, please call GetEnumerator for [LiongPlus::Collections::IEnumerator].
		/// </summary>
		/// <returns>Parsed [LiongPlus::Collections::IEnumerator] which at the end of the collection.</returns>
		EnumeratorParser<T> end()
		{
			return new TEnumerator(_Ptr, _Size, _Size);
		}

		Ptr<Array<T>> Clone() const
		{
			Ptr<Array<T>> clone = new Array<T>(_Size);
			Copy(*this, *clone, _Size);
			return Array<T>(*this);
		}

		long GetLowerBound() const
		{
			return 0;
		}

		T* GetNativePointer() const
		{
			return _Ptr;
		}

		long GetUpperBound() const
		{
			return _Size - 1;
		}

		T& GetValue(long index) const
		{
			assert(index >= 0, "Need non-negative number.");
			assert(index < _Size, "Bound exceeded.");

			return _Ptr[index];
		}
		
		// Static Members

		static long BinarySearch(Array<T>& arr, long index, long length, T value, Ptr<IComparer<T>> comparer)
		{
			assert(index >= 0 && length >= 0, "Need non-negative number.");
			assert(index + length <= _Size, "Bound exceeded.");
			assert(comparer != nullptr, "$comparer is nullptr.");

			return BinarySearchImpl(arr, index, length, value, comparer);
		}
		static long BinarySearch(Array<T>& arr, long index, long length, T value)
		{
			return BinarySearch(arr, index, length, value, new DefaultComparer());
		}
		static long BinarySearch(Array<T>& arr, T value, Ptr<IComparer<T>> comparer)
		{
			return BinarySearch(arr, 0, _Size, value, comparer);
		}
		/// <summary>
		/// Searches an entire one-dimensional sorted array for a specific element, using [operator>] and [operator<] overrided by each element of the Array and by the specified object.
		/// </summary>
		/// <param name="value">The object to search for.</param>
		/// <returns>The index of the specified value in the specified array, if value is found. If value is not found and value is less than one or more elements in array, a negative number which is the bitwise complement of the index of the first element that is larger than value. If value is not found and value is greater than any of the elements in array, a negative number which is the bitwise complement of (the index of the last element plus 1).</returns>
		static long BinarySearch(Array<T>& arr, T value)
		{
			return BinarySearch(arr, 0, _Size, value, new DefaultComparer());
		}

		static void Clear(Array<T>& arr, long index, long length)
		{
			assert(arr != nullptr, "$arr is nullptr.");
			assert(index >= 0, "Need non-negative number.");
			assert(index + length <= arr._Size, "Bound exceeded.");

			while (length-- > 0)
				*((byte*)(arr._Ptr + index + length) - 1) = 0;
		}

		template<typename TOutput>
		static Array<TOutput> ConvertAll(Array<T>& arr, Converter<T, TOutput> converter)
		{
			Array<TOutput> newArr(arr._Size);
			long size = arr._Size;
			while (size-- > 0)
			{
				*(newArr._Ptr + size) = *(dynamic_cast<TOutput*>(converter._Ptr + size));
			}
			return newArr;
		}

		static void Copy(const Array<T>& sourceArray, long sourceIndex, const Array<T>& destinationArray, long destinationIndex, long length)
		{
			assert(sourceIndex >= 0 && destinationIndex >= 0 && length >= 0, "Need non-negative number.");
			assert(sourceIndex + length <= sourceArray._Size && destinationIndex + length <= destinationArray._Size, "Bound exceeded.");

			while (length-- > 0)
				*(destinationArray._Ptr + length) = *(sourceArray._Ptr + length);
		}
		static void Copy(const Array<T>& sourceArray, const Array<T>& destinationArray, long length)
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
			for (long i = 0; i < arr._Size; ++i)
			{
				if (match(arr._Ptr[i]))
					return true;
			}
			return false;
		}

		static T Find(Array<T>& arr, Predicate<T>& match)
		{
			assert(match != nullptr, "$match is nullptr.");

			for (long i = 0; i < arr._Size; ++i)
			{
				if (match(arr._Ptr[i]))
					return arr._Ptr[i];
			}
			return T();
		}

		static Array<T> FindAll(Array<T>& arr, Predicate<T>& match)
		{
			assert(match != nullptr, "$match is nullptr.");

			throw NotImplementedException("[FindAll] is not implemented yet.");
		}

		static long FindIndex(Array<T>& arr, long startIndex, long count, Predicate<T>& match)
		{
			assert(startIndex >= 0, "Need non-negative number.");
			assert(startIndex + count <= arr._Size, "Bound exceeded.");
			assert(match != nullptr, "$match is nullptr.");

			for (long i = 0; i < count; ++i)
			{
				if (match(arr._Ptr[i]))
					return i;
			}
			return -1;
		}
		static long FindIndex(Array<T>& arr, long startIndex, Predicate<T>& match)
		{
			FindIndex(startIndex, arr._Size - startIndex, match);
		}
		static long FindIndex(Array<T>& arr, Predicate<T>& match)
		{
			FindIndex(0, match);
		}

		static T FindLast(Array<T>& arr, Predicate<T>& match)
		{
			assert(match != nullptr, "$match is nullptr.");

			for (long i = arr._Size - 1; i >= 0; --i)
			{
				if (match(arr._Ptr[i]))
					return arr._Ptr[i];
			}
			return T();
		}

		static long FindLastIndex(Array<T>& arr, long startIndex, long count, Predicate<T>& match)
		{
			assert(startIndex >= 0, "Need non-negative number.");
			assert(startIndex + count <= arr._Size, "Bound exceeded.");
			assert(match != nullptr, "$match is nullptr.");

			for (long i = arr._Size - 1; i >= 0; --i)
			{
				if (match(arr._Ptr[i]))
					return i;
			}
			return -1;
		}
		static long FindLastIndex(Array<T>& arr, long startIndex, Predicate<T>& match)
		{
			FindIndex(startIndex, arr._Size - startIndex, match);
		}
		static long FindLastIndex(Array<T>& arr, Predicate<T>& match)
		{
			FindIndex(0, match);
		}

		static void ForEach(Action<T>& action)
		{
			assert(match != nullptr, "$action is nullptr.");

			for (long i = 0; i < _Size; ++i)
				action(_Ptr[i]);
		}

		static long IndexOf(Array<T> arr, T& value)
		{
			throw NotImplementedException();
		}

		/// <summary>
		/// Sorts a range of elements in $items based on [this] using the specified [LiongPlus::Collections::IComparer].
		/// </summary>
		/// <param name="items">
		/// The [LiongPlus::Collections::Array] that contains the items that correspond to each of the keys in [this].
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
		static void Sort(Array<T>& keys, Array<TValue>& items, long index, long length, Ptr<IComparer<T>> comparer)
		{
			assert(index >= 0 && length >= 0, "Need non-negative number.");
			assert(index + length <= keys._Size, "Bound exceeded.");
			assert(items.GetCount() != 0 && items.GetCount() >= index + length, "$items is too short."); // Sort for pairs
			assert(comparer != nullptr, "Null comparer.");

			SortImpl(keys, items, index, length, comparer, !items.GetCount());
		}
		/// <summary>
		/// Sorts a range of elements in $items based on [this].
		/// </summary>
		/// <param name="items">
		/// The [LiongPlus::Collections::Array] that contains the items that correspond to each of the keys in [this].
		/// -or-
		/// nullptr to sort only the keys Array.
		/// </param>
		/// <param name="index">The starting index of the range to sort.</param>
		/// <param name="length">The number of elements in the range to sort.</param>
		/// <typeparam name="TValue">The type of the elements of the items array.</typeparam>
		template<typename TValue>
		static void Sort(Array<T>& keys, Array<TValue>& items, long index, long length)
		{
			Sort(keys, items, index, length, new DefaultComparer());
		}
		/// <summary>
		/// Sorts elements in $items based on [this] using the specified [LiongPlus::Collections::IComparer].
		/// </summary>
		/// <param name="items">
		/// The [LiongPlus::Collections::Array] that contains the items that correspond to each of the keys in [this].
		/// -or-
		/// nullptr to sort only the keys Array.
		/// </param>
		/// <param name="comparer">
		/// The [LiongPlus::Collections::IComparer] implementation to use when comparing elements.
		/// -or-
		/// nullptr to use the default implementation of each element.
		/// </param>
		/// <typeparam name="TValue">The type of the elements of the items array.</typeparam>
		template<typename TValue>
		static void Sort(Array<T>& keys, Array<TValue>& items, Ptr<IComparer<T>> comparer)
		{
			Sort(keys, items, 0, _Size, comparer);
		}
		/// <summary>
		/// Sorts elements in $items based on [this].
		/// </summary>
		/// <param name="items">
		/// The [LiongPlus::Collections::Array] that contains the items that correspond to each of the keys in [this].
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
		/// Sorts the elements in a range of elements in an Array using the specified [LiongPlus::Collections::IComparer].
		/// </summary>
		/// <param name="index">The starting index of the range to sort.</param>
		/// <param name="length">The number of elements in the range to sort.</param>
		/// <param name="comparer">
		/// The [LiongPlus::Collections::IComparer] implementation to use when comparing elements.
		/// -or-
		/// nullptr to use the default implementation of each element..</param>
		static void Sort(Array<T>& arr, long index, long length, Ptr<IComparer<T>> comparer)
		{
			Sort(arr, Array<T>(nullptr), index, length, comparer);
		}
		/// <summary>
		/// Sorts the elements in a range of elements.
		/// </summary>
		/// <param name="index">The starting index of the range to sort.</param>
		/// <param name="length">The number of elements in the range to sort.</param>
		static void Sort(Array<T>& arr, long index, long length)
		{
			Sort(arr, index, length, new DefaultComparer());
		}
		/// <summary>
		/// Sorts the elements.
		/// </summary>
		static void Sort(Array<T>& arr, Ptr<IComparer<T>> comparer)
		{
			Sort(arr, 0, _Size, comparer);
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

			for (long i = 0; i < arr._Size; ++i)
			{
				if (!match(arr._Ptr[i]))
					return false;
			}
			return true;
		}

		// IBuffer

		const Byte* AbandonBuffer()
		{
			const Byte* ptr = reinterpret_cast<const Byte*>(_Ptr);
			_Ptr = new T[_Size];
			return ptr;
		}

		const Byte* AccessBuffer()
		{
			return reinterpret_cast<const Byte*>(_Ptr);
		}

		bool IsBufferAccessable()
		{
			return true;
		}

		// ICollection<T>

		virtual long Add(T& value) override final
		{
			throw NotSupportedException("Fixed size collection.");
		}

		virtual void Clear() override final
		{
			long size = _Size;
			CleanUp();
			_Size = size;
			_Ptr = new T[size];
		}

		virtual bool Contains(T& value) override final
		{
			for (auto v : *this)
			{
				if (v == value)
					return true;
			}
			return false;
		}

		virtual long GetCount() override final
		{
			return _Size;
		}

		virtual void CopyTo(Array<T>& array, long index) override final
		{
			Copy(*this, 0, array, index, _Size);
		}

		virtual void Remove(T& value) override final
		{
			throw NotSupportedException("Fixed size collection.");
		}

		// IEnumerable<T>.

		virtual Ptr<IEnumerator<T>> GetEnumerator() override final
		{
			return new TEnumerator(_Ptr, _Size, 0);
		}

		// IList<T>.
		
		virtual long IndexOf(T& value) override final
		{
			for (long i = 0; i < _Size; ++i)
			{
				if (GetValue(i) == value)
					return i;
			}
			return -1;
		}

		virtual void Insert(long index, T& value) override final
		{
			throw NotSupportedException("Fixed size collection.");
		}

		virtual void RemoveAt(long index) override final
		{
			throw NotSupportedException("Fixed size collection.");
		}

		// Private

	private:
		typedef ContinuousMemoryEnumerator<T> TEnumerator;

		class DefaultComparer
			: public Object
			, public IComparer<T>
		{
		public:
			DefaultComparer()
			{
			}
			~DefaultComparer()
			{
			}

			virtual long Compare(T& x, T& y) override
			{
				return x == y
					? 0
					: x <= y
					? -1
					: 1;
			}
		};

		long _Size;
		T* _Ptr;
		ReferenceCounter* _Counter;

		static long BinarySearchImpl(Array<T>& arr, long index, long length, T& value, Ptr<IComparer<T>> comparer)
		{
			if (arr._Ptr[index] == value)
				return index;

			if (length <= 1)
				return -1;

			long mid = length / 2;

			if (comparer->Compare(value, arr._Ptr[index + mid]))
				return BinarySearchImpl(arr, value, index, mid);
			else
				return BinarySearchImpl(arr, value, index + mid, length - mid);
		}

		template<typename TValue>
		static void SortImpl(Array<T>& keys, Array<TValue>& items, long index, long length, Ptr<IComparer<T>> comparer, bool itemsIsNull)
		{
			if (--length < 1) return;

			long left = index, right = index + length, pivot = left++;
			while (left != right)
			{
				if (comparer->Compare(keys._Ptr[left], keys._Ptr[pivot]) <= 0)
					++left;
				else
				{
					while (left != right && comparer->Compare(keys._Ptr[pivot], keys._Ptr[right]) <= 0)
						--right;
					Swap(keys._Ptr[left], keys._Ptr[right]);
					if (!itemsIsNull)
						Swap(items._Ptr[left], items._Ptr[right]);
				}
			}

			if (comparer->Compare(keys._Ptr[pivot], keys._Ptr[left]) <= 0)
				--left;

			Swap(keys._Ptr[left], keys._Ptr[pivot]);
			if (!itemsIsNull)
				Swap(items._Ptr[left], items._Ptr[pivot]);

			SortImpl(keys, items, index, left - index + 1, comparer, itemsIsNull);
			SortImpl(keys, items, right, length - right + 1, comparer, itemsIsNull);
		}

		virtual void CleanUp()
		{
			_Size = 0;
			if (_Counter && !_Counter->Dec())
			{
				if (_Ptr)
				{
					delete [] _Ptr;
					_Ptr = nullptr;
				}
				delete _Counter;
				_Counter = nullptr;
			}
		}
	};
}
#endif