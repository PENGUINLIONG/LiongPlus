// File: List.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_List
#define _L_List
#include "../Fundamental.hpp"
#include "../Array.hpp"
#include "../Buffer.hpp"
#include "../Delegate.hpp"
#include "../Exception.hpp"
#include "ICollection.hpp"
#include "IComparer.hpp"
#include "IEnumerable.hpp"
#include "IEnumerator.hpp"
#include "IList.hpp"
#include "../Ptr.hpp"

namespace LiongPlus
{
	namespace Collections
	{
		template<typename T>
		class List
			: public Object
			, public IList<T>
		{
			// Public

		public:
			List()
				: _Data(new T[_InitialCapacity], _InitialCapacity)
				, _Count(new int(0))
			{
			}
			List(const List<T>& instance)
				: _Data(instance._Data)
				, _Count(instance._Count)
			{
			}
			List(List<T>&& instance)
				: _Data()
				, _Count(nullptr)
			{
				Swap(_Data, instance._Data);
				Swap(_Count, instance._Count);
			}
			List(IEnumerable<T>* source)
				: List()
			{
				AddRange(source);
			}
			List(int capacity)
				: _Data(new T[capacity])
				, _Count(new int(0))
			{
			}
			~List()
			{
			}

			List<T>& operator=(const List<T>& instance)
			{
				_Data = instance._Data;
				_Count = instance._Count;
			}
			List<T>& operator=(List<T>&& instance)
			{
				Swap(_Data, instance._Data);
				Swap(_Count, instance._Count);
			}

			T& operator[](int index)
			{
				return _Data[index];
			}

			/// <summary>
			/// Take the advantage of C++11 range-based loop. Never call this method, please call GetEnumerator for [LiongPlus::Collections::IEnumerator].
			/// </summary>
			/// <returns>Parsed [LiongPlus::Collections::IEnumerator] which at the end of the collection.</returns>
			EnumeratorParser<T> begin()
			{
				return _Data.begin();
			}
			/// <summary>
			/// Take the advantage of C++11 range-based loop. Never call this method, please call GetEnumerator for [LiongPlus::Collections::IEnumerator].
			/// </summary>
			/// <returns>Parsed [LiongPlus::Collections::IEnumerator] which at the end of the collection.</returns>
			EnumeratorParser<T> end()
			{
				return new ContinuousMemoryEnumerator<T>(_Data.GetNativePointer(), *_Count, *_Count);
			}

			int AddRange(IEnumerable<T>* source)
			{
				int count = 0;
				Ptr<IEnumerator<T>> e = source->GetEnumerator();

				while (e->MoveNext())
				{
					Add(source->GetEnumerator()->Current());
					++count;
				}
				return count;
			}
			int AddRange(std::initializer_list<T> source)
			{
				int count = 0;
				for (auto& t : source)
				{
					Add((RemoveConst<T>::Type)t);
					++count;
				}

				return count;
			}
			T& First()
			{
				return _Data[0];
			}
			int GetCapacity()
			{
				return _Data.GetLength();
			}
			int InsertRange(int index, IEnumerable<T>* source)
			{
				int count = 0;
				Ptr<IEnumerator<T>> e = source->GetEnumerator();
				// Make room for new data.
				while (e->MoveNext())
					++count;
				Expand(count);
				Array<T>::Copy(_Data, index, _Data, index + count, *_Count - index);
				// Insert.
				e->Reset();
				count = 0;
				while (e->MoveNext())
					_Data[insert + (count++)] = e->Current();
				return count;
			}
			T& Last()
			{
				return _Data[*_Count - 1];
			}
			bool SetCapacity(int value)
			{
				if (value <= _Data.GetLength())
					return false;
				Array<T> newData(value);
				_Data.CopyTo(newData, 0);
				_Data = newData;
				return true;
			}
			void Sort()
			{
				Array<T>::Sort(_Data);
			}
			void Sort(IComparer<T> comparer)
			{
				Array<T>::Sort(_Data, comparer);
			}
			void Sort(int index, int count, IComparer<T>& comparer)
			{
				Array<T>::Sort(_Data, index, count, comparer);
			}
			void Sort(Comparison<T> comparison)
			{
				Array<T>::Sort(_Data, FunctorComparer<T>(comparison));
			}
			
			Array<T> ToArray()
			{
				Array<T>(_Count) arr;
				Array<T>::Copy(_Data, arr, _Count);
				return arr;
			}

			// IList<T>

			virtual int Add(T& value) override
			{
				Expand(1);
				_Data[(*_Count)++] = value;
				return (*_Count) - 1;
			}
			virtual void Clear() override
			{
				_Data.Clear();
			}
			virtual bool Contains(T& value) override
			{
				int count = *_Count;
				while (count-- > 0)
				{
					if (value == _Data[count])
						return true;
				}
				return false;
			}
			virtual int GetCount() override
			{
				return *_Count;
			}
			virtual void CopyTo(Array<T>& array, int index) override
			{
				assert(array.GetLength() < *_Count, "array is not long enough");
				_Data.CopyTo(array, index);
			}
			virtual void Insert(int index, T& value) override
			{
				Expand(1);
				Array<T>::Copy(_Data, index, _Data, index + 1, *_Count - 1);
				_Data[index] = value;
				++(*_Count);
			}
			virtual bool IsReadOnly() override
			{
				return false;
			}
			virtual void Remove(T& value) override
			{
				int i = *_Count;
				while (i-- > 0)
				{
					if (_Data[i] == value)
					{
						break;
						i = -1;
					}
				}
				if (i >= 0)
				{
					Array<T>::Copy(_Data, i + 1, _Data, i, *_Count - i);
					_Data[(*_Count)--] = T();
				}
			}
			virtual Ptr<IEnumerator<T>> GetEnumerator() override
			{
				return _Data.GetEnumerator();
			}
			virtual int IndexOf(T& item) override
			{
				int i = *_Count;
				while (i-- > 0)
				{
					if (_Data[i] == item)
						return i;
				}
				return -1;
			}
			virtual void RemoveAt(int index) override
			{
				assert(index < 0 || index > *_Count, "index");
				Array<T>::Copy(_Data, index + 1, _Data, index, *_Count - index);
				_Data[(*_Count)--] = T();
			}

			// Private

		private:
			Array<T> _Data;
			Ptr<int> _Count;

			const static int _InitialCapacity = 4;

			void Expand(int length)
			{
				if (*_Count + length > _Data.GetLength())
					SetCapacity(*_Count + length + 4);
			}
		};
	}
}
#endif