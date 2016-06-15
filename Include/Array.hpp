// File: Array.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Array
#define _L_Array
#include "Fundamental.hpp"

namespace LiongPlus
{
	template<typename T>
	class Array
	{
	public:
		Array()
			: _Size(0)
			, _Ptr(nullptr)
		{
		}
<<<<<<< HEAD
		Array(const size_t size)
=======
		Array(const long size)
>>>>>>> master
			: _Size(size)
			, _Ptr(new T[size])
		{
		}
<<<<<<< HEAD
		Array(const T* pointer, size_t size)
=======
		Array(const T* pointer, long size)
>>>>>>> master
			: _Size(size)
			, _Ptr(new T[size])
		{
			while (size-- > 0)
				*(_Ptr + size) = *(pointer + size);
		}
		Array(const Array<T>& instance)
			: _Size(instance._Size)
			, _Ptr(new T[instance._Size])
		{
			Copy(instance, *this, 0);
		}
		Array(Array<T>&& instance)
			: Array()
		{
			swap(_Size, instance._Size);
			swap(_Ptr, instance._Ptr);
		}
		Array(const std::initializer_list<T> initList)
			: _Size(initList.size())
			, _Ptr(new T[initList.size()])
		{
<<<<<<< HEAD
			size_t i = 0;
=======
			long i = 0;
>>>>>>> master
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
			return *this;
		}
		Array<T>& operator=(Array<T>&& instance)
		{
			swap(_Size, instance._Size);
			swap(_Ptr, instance._Ptr);
			return *this;
		}
		Array<T>& operator=(std::initializer_list<T>& initList)
		{
			CleanUp();
			_Size = initList.size();
			T* field = new T[_Size];
<<<<<<< HEAD
			size_t i = 0;
=======
			long i = 0;
>>>>>>> master
			for (auto t : initList)
				field[i++] = t;
			_Ptr = field;
			return *this;
		}

<<<<<<< HEAD
		T& operator[](size_t index) const
=======
		T& operator[](long index) const
>>>>>>> master
		{
			assert(index >= 0 && index < _Size, "index");

			return _Ptr[index];
		}

		// Take the advantage of C++11 range-based loop.
		ArrayIterator<T> begin() const
		{
			return ArrayIterator<T>(_Ptr);
		}
		ArrayIterator<T> end() const
		{
			return ArrayIterator<T>(_Ptr + _Size);
		}
		
		void Clear()
		{
			size_t count = _Size;
			while (count-- > 0)
				_Ptr[count] = T();
		}

<<<<<<< HEAD
		bool Contains(T& value)
=======
		long GetLowerBound() const
>>>>>>> master
		{
			for (auto v : *this)
			{
				if (v == value)
					return true;
			}
			return false;
		}
				
		void CopyTo(Array<T>& array, size_t index)
		{
			Copy(*this, 0, array, index, _Size);
		}
<<<<<<< HEAD
		
		size_t GetCount()
=======

		long GetUpperBound() const
>>>>>>> master
		{
			return _Size;
		}
		
		T* GetNativePointer() const
		{
			return _Ptr;
		}

<<<<<<< HEAD
		T& GetValue(size_t index) const
=======
		T& GetValue(long index) const
>>>>>>> master
		{
			assert(index >= 0, "Need non-negative number.");
			assert(index < _Size, "Bound exceeded.");

			return _Ptr[index];
		}
		
		size_t IndexOf(T& value)
		{
			for (size_t i = 0; i < _Size; ++i)
			{
				if (GetValue(i) == value)
					return i;
			}
			return -1;
		}
		
		// Private

<<<<<<< HEAD
	private:
		class ArrayIterator
		{
		private:
			const T* _Pos;

		public:
			ArrayIterator(T* pos) : _Pos(pos) { }
			~ArrayIterator() { }

			T& operator*() { return *_Pos; }
			bool operator!=(ArrayIterator<T>* value) { return &value->_Pos != &_Pos; }
			ArrayIterator& operator++() { return ++_Pos, *this; }
		};
	
		typedef ContinuousMemoryEnumerator<T> TEnumerator;

		size_t _Size;
		T* _Ptr;
		
		void CleanUp()
=======
		static long BinarySearch(Array<T>& arr, long index, long length, T value, Ptr<IComparer<T>> comparer)
		{
			assert(index >= 0 && length >= 0, "Need non-negative number.");
			assert(index + length <= _Size, "Bound exceeded.");
			assert(comparer != nullptr, "$comparer is nullptr.");

			return BinarySearchImpl(arr, index, length, value, comparer);
		}
		static long BinarySearch(Array<T>& arr, long index, long length, T value)
>>>>>>> master
		{
			_Size = 0;
			if (_Ptr)
			{
				delete [] _Ptr;
				_Ptr = nullptr;
			}
		}
<<<<<<< HEAD
	};
	
	template<typename T>
	class ArrayUtil
	{
	public:
		static size_t BinarySearch(Array<T>& arr, size_t index, size_t length, T value)
=======
		static long BinarySearch(Array<T>& arr, T value, Ptr<IComparer<T>> comparer)
>>>>>>> master
		{
			assert(index < 0 || length < 0, "Need non-negative number.");
			assert(index + length > _Size, "Bound exceeded.");
			assert(comparer == nullptr, "$comparer is nullptr.");

			return BinarySearchImpl(arr, index, length, values);
		}
		/// <summary>
		/// Searches an entire one-dimensional sorted array for a specific element, using [operator>] and [operator<] overrided by each element of the Array and by the specified object.
		/// </summary>
		/// <param name="value">The object to search for.</param>
		/// <returns>The index of the specified value in the specified array, if value is found. If value is not found and value is less than one or more elements in array, a negative number which is the bitwise complement of the index of the first element that is larger than value. If value is not found and value is greater than any of the elements in array, a negative number which is the bitwise complement of (the index of the last element plus 1).</returns>
<<<<<<< HEAD
		static size_t BinarySearch(Array<T>& arr, T value)
=======
		static long BinarySearch(Array<T>& arr, T value)
>>>>>>> master
		{
			return BinarySearch(arr, 0, _Size, value);
		}

<<<<<<< HEAD
		static void Clear(Array<T>& arr, size_t index, size_t length)
=======
		static void Clear(Array<T>& arr, long index, long length)
>>>>>>> master
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
<<<<<<< HEAD
			size_t size = arr._Size;
=======
			long size = arr._Size;
>>>>>>> master
			while (size-- > 0)
			{
				*(newArr._Ptr + size) = *(dynamic_cast<TOutput*>(converter._Ptr + size));
			}
			return newArr;
		}

<<<<<<< HEAD
		static void Copy(const Array<T>& sourceArray, size_t sourceIndex, const Array<T>& destinationArray, size_t destinationIndex, size_t length)
=======
		static void Copy(const Array<T>& sourceArray, long sourceIndex, const Array<T>& destinationArray, long destinationIndex, long length)
>>>>>>> master
		{
			assert(sourceIndex >= 0 && destinationIndex >= 0 && length >= 0, "Need non-negative number.");
			assert(sourceIndex + length <= sourceArray._Size && destinationIndex + length <= destinationArray._Size, "Bound exceeded.");

			while (length-- > 0)
				*(destinationArray._Ptr + length) = *(sourceArray._Ptr + length);
		}
<<<<<<< HEAD
		static void Copy(const Array<T>& sourceArray, const Array<T>& destinationArray, size_t length)
=======
		static void Copy(const Array<T>& sourceArray, const Array<T>& destinationArray, long length)
>>>>>>> master
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
<<<<<<< HEAD
			for (size_t i = 0; i < arr._Size; ++i)
=======
			for (long i = 0; i < arr._Size; ++i)
>>>>>>> master
			{
				if (match(arr._Ptr[i]))
					return true;
			}
			return false;
		}

		static T Find(Array<T>& arr, Predicate<T>& match)
		{
			assert(match != nullptr, "$match is nullptr.");

<<<<<<< HEAD
			for (size_t i = 0; i < arr._Size; ++i)
=======
			for (long i = 0; i < arr._Size; ++i)
>>>>>>> master
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

<<<<<<< HEAD
		static size_t FindIndex(Array<T>& arr, size_t startIndex, size_t count, Predicate<T>& match)
=======
		static long FindIndex(Array<T>& arr, long startIndex, long count, Predicate<T>& match)
>>>>>>> master
		{
			assert(startIndex >= 0, "Need non-negative number.");
			assert(startIndex + count <= arr._Size, "Bound exceeded.");
			assert(match != nullptr, "$match is nullptr.");

<<<<<<< HEAD
			for (size_t i = 0; i < count; ++i)
=======
			for (long i = 0; i < count; ++i)
>>>>>>> master
			{
				if (match(arr._Ptr[i]))
					return i;
			}
			return -1;
		}
<<<<<<< HEAD
		static size_t FindIndex(Array<T>& arr, size_t startIndex, Predicate<T>& match)
		{
			FindIndex(startIndex, arr._Size - startIndex, match);
		}
		static size_t FindIndex(Array<T>& arr, Predicate<T>& match)
=======
		static long FindIndex(Array<T>& arr, long startIndex, Predicate<T>& match)
		{
			FindIndex(startIndex, arr._Size - startIndex, match);
		}
		static long FindIndex(Array<T>& arr, Predicate<T>& match)
>>>>>>> master
		{
			FindIndex(0, match);
		}

		static T FindLast(Array<T>& arr, Predicate<T>& match)
		{
			assert(match != nullptr, "$match is nullptr.");

<<<<<<< HEAD
			for (size_t i = arr._Size - 1; i >= 0; --i)
=======
			for (long i = arr._Size - 1; i >= 0; --i)
>>>>>>> master
			{
				if (match(arr._Ptr[i]))
					return arr._Ptr[i];
			}
			return T();
		}

<<<<<<< HEAD
		static size_t FindLastIndex(Array<T>& arr, size_t startIndex, size_t count, Predicate<T>& match)
=======
		static long FindLastIndex(Array<T>& arr, long startIndex, long count, Predicate<T>& match)
>>>>>>> master
		{
			assert(startIndex >= 0, "Need non-negative number.");
			assert(startIndex + count <= arr._Size, "Bound exceeded.");
			assert(match != nullptr, "$match is nullptr.");

<<<<<<< HEAD
			for (size_t i = arr._Size - 1; i >= 0; --i)
=======
			for (long i = arr._Size - 1; i >= 0; --i)
>>>>>>> master
			{
				if (match(arr._Ptr[i]))
					return i;
			}
			return -1;
		}
<<<<<<< HEAD
		static size_t FindLastIndex(Array<T>& arr, size_t startIndex, Predicate<T>& match)
		{
			FindIndex(startIndex, arr._Size - startIndex, match);
		}
		static size_t FindLastIndex(Array<T>& arr, Predicate<T>& match)
=======
		static long FindLastIndex(Array<T>& arr, long startIndex, Predicate<T>& match)
		{
			FindIndex(startIndex, arr._Size - startIndex, match);
		}
		static long FindLastIndex(Array<T>& arr, Predicate<T>& match)
>>>>>>> master
		{
			FindIndex(0, match);
		}

		static void ForEach(Action<T>& action)
		{
			assert(match != nullptr, "$action is nullptr.");

<<<<<<< HEAD
			for (size_t i = 0; i < _Size; ++i)
				action(_Ptr[i]);
		}

		static size_t IndexOf(Array<T> arr, T& value)
=======
			for (long i = 0; i < _Size; ++i)
				action(_Ptr[i]);
		}

		static long IndexOf(Array<T> arr, T& value)
>>>>>>> master
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
<<<<<<< HEAD
		static void Sort(Array<T>& keys, Array<TValue>& items, size_t index, size_t length)
		{
			size_t itemsLength = 0;
			if (items != nullptr)
				itemsLength = items.GetCount();

			assert(index < 0 || length < 0, "Need non-negative number.");
			assert(index + length > keys._Size, "Bound exceeded.");
			assert(itemsLength && itemsLength < index + length, "$items is too short."); // Sort for pairs
			assert(comparer == nullptr, "Null comparer.");

			SortImpl(keys, items, index, length, itemsLength != 0);
=======
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
>>>>>>> master
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
<<<<<<< HEAD
=======
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
>>>>>>> master
		/// Sorts the elements in a range of elements.
		/// </summary>
		/// <param name="index">The starting index of the range to sort.</param>
		/// <param name="length">The number of elements in the range to sort.</param>
<<<<<<< HEAD
		static void Sort(Array<T>& arr, size_t index, size_t length)
=======
		static void Sort(Array<T>& arr, long index, long length)
		{
			Sort(arr, index, length, new DefaultComparer());
		}
		/// <summary>
		/// Sorts the elements.
		/// </summary>
		static void Sort(Array<T>& arr, Ptr<IComparer<T>> comparer)
>>>>>>> master
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

<<<<<<< HEAD
			for (size_t i = 0; i < arr._Size; ++i)
=======
			for (long i = 0; i < arr._Size; ++i)
>>>>>>> master
			{
				if (!match(arr._Ptr[i]))
					return false;
			}
			return true;
		}
<<<<<<< HEAD
	private:
		static size_t BinarySearchImpl(Array<T>& arr, size_t index, size_t length, T& value)
=======

		// IBuffer

		Byte* AbandonBuffer()
		{
			Byte* ptr = reinterpret_cast<Byte*>(_Ptr);
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
>>>>>>> master
		{
			if (arr._Ptr[index] == value)
				return index;

			if (length <= 1)
				return -1;

<<<<<<< HEAD
			size_t mid = length / 2;
=======
			long mid = length / 2;
>>>>>>> master

			if (arr._Ptr[index + mid] <= value)
				return BinarySearchImpl(arr, index, mid, value);
			else
				return BinarySearchImpl(arr, index + mid, length - mid, value);
		}

		template<typename TValue>
<<<<<<< HEAD
		static void SortImpl(Array<T>& keys, Array<TValue>& items, size_t index, size_t length, bool itemsIsNull)
		{
			if (--length < 1) return;

			size_t left = index, right = index + length, pivot = left++;
=======
		static void SortImpl(Array<T>& keys, Array<TValue>& items, long index, long length, Ptr<IComparer<T>> comparer, bool itemsIsNull)
		{
			if (--length < 1) return;

			long left = index, right = index + length, pivot = left++;
>>>>>>> master
			while (left != right)
			{
				if (keys._Ptr[left] <= keys._Ptr[pivot])
					++left;
				else
				{
					while (left != right && (keys._Ptr[pivot] <= keys._Ptr[right])
						--right;
					swap(keys._Ptr[left], keys._Ptr[right]);
					if (!itemsIsNull)
						swap(items._Ptr[left], items._Ptr[right]);
				}
			}

			if (keys._Ptr[pivot] <= keys._Ptr[left])
				--left;

			swap(keys._Ptr[left], keys._Ptr[pivot]);
			if (!itemsIsNull)
				swap(items._Ptr[left], items._Ptr[pivot]);

			SortImpl(keys, items, index, left - index + 1, comparer, itemsIsNull);
			SortImpl(keys, items, right, length - right + 1, comparer, itemsIsNull);
		}
	};
}
#endif