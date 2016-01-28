// File: StringBuilder.cpp
// Author: Rendong Liang (Liong)

#include "StringBuilder.hpp"

namespace LiongPlus
{
	namespace Text
	{
		// Public

		StringBuilder::StringBuilder()
			: _Next(nullptr)
			, _Counter(new ReferenceCounter())
			, _Capacity(_InitialCapacity)
			, _Length(0)
			, _Data(new _L_Char[_InitialCapacity] ,_InitialCapacity)
		{
			_Counter->Inc();
		}
		StringBuilder::StringBuilder(long capacity)
			: _Next(nullptr)
			, _Counter(new ReferenceCounter())
			, _Capacity(capacity)
			, _Length(0)
			, _Data(new _L_Char[capacity], capacity)
		{
			_Counter->Inc();
		}
		StringBuilder::StringBuilder(String& str)
			: StringBuilder(str.GetNativePointer(), str.GetLength() - 1)
		{
		}
		StringBuilder::StringBuilder(const StringBuilder& instance)
			: _Next(instance._Next == nullptr ? nullptr : new StringBuilder(*instance._Next))
			, _Counter(instance._Counter)
			, _Capacity(instance._Capacity)
			, _Length(instance._Length)
			, _Data(new _L_Char[instance._Capacity], instance._Capacity)
		{
			_Counter->Inc();
			Array<_L_Char>::Copy(_Data, instance._Data, _Length);
		}
		StringBuilder::StringBuilder(StringBuilder&& instance)
			: _Next(nullptr)
			, _Counter(nullptr)
			, _Capacity(0)
			, _Length(0)
			, _Data()
		{
			Swap(_Next, instance._Next);
			Swap(_Counter, instance._Counter);
			Swap(_Capacity, instance._Capacity);
			Swap(_Length, instance._Length);
			Swap(_Data, instance._Data);
		}
		StringBuilder::StringBuilder(const _L_Char* c_str)
			: StringBuilder(c_str, Buffer::Wcslen(c_str))
		{
		}
		StringBuilder::~StringBuilder()
		{
			if (_Counter && !_Counter->Dec())
			{
				if (_Next)
				{
					delete _Next;
					_Next = nullptr;
				}
				_Counter = nullptr;
			}
		}

		StringBuilder& StringBuilder::operator=(const StringBuilder& instance)
		{
			_Next = instance._Next == nullptr ? nullptr : new StringBuilder(*instance._Next);
			_Counter = instance._Counter;
			_Capacity = instance._Capacity;
			_Length = instance._Length;
			_Data = Array<_L_Char>(new _L_Char[instance._Capacity], instance._Capacity);
			if (_Counter != nullptr)
				_Counter->Inc();
			Array<_L_Char>::Copy(_Data, instance._Data, _Length);
			return *this;
		}
		StringBuilder& StringBuilder::operator=(StringBuilder&& instance)
		{
			Swap(_Next, instance._Next);
			Swap(_Counter, instance._Counter);
			Swap(_Capacity, instance._Capacity);
			Swap(_Length, instance._Length);
			Swap(_Data, instance._Data);
			return *this;
		}

		StringBuilder& StringBuilder::Append(_L_Char c)
		{
			StringBuilder* ptr = this;

			ReachEndOfStringBuilderChain(ptr);
			Expand(1);
			ptr->_Data[ptr->_Length - 1] = c;
			return *this;
		}
		StringBuilder& StringBuilder::Append(signed char value)
		{
			return Append(String::FromValue(value));
		}
		StringBuilder& StringBuilder::Append(unsigned char value)
		{
			return Append(String::FromValue(value));
		}
		StringBuilder& StringBuilder::Append(short value)
		{
			return Append(String::FromValue(value));
		}
		StringBuilder& StringBuilder::Append(unsigned short value)
		{
			return Append(String::FromValue(value));
		}
		StringBuilder& StringBuilder::Append(int value)
		{
			return Append(String::FromValue(value));
		}
		StringBuilder& StringBuilder::Append(unsigned int value)
		{
			return Append(String::FromValue(value));
		}
		StringBuilder& StringBuilder::Append(long value)
		{
			return Append(String::FromValue(value));
		}
		StringBuilder& StringBuilder::Append(unsigned long value)
		{
			return Append(String::FromValue(value));
		}
		StringBuilder& StringBuilder::Append(long long value)
		{
			return Append(String::FromValue(value));
		}
		StringBuilder& StringBuilder::Append(unsigned long long value)
		{
			return Append(String::FromValue(value));
		}
		StringBuilder& StringBuilder::Append(String& str)
		{
			return Append(str.GetNativePointer(), str.GetLength() - 1);
		}
		StringBuilder& StringBuilder::Append(const _L_Char* c_str, long length)
		{
			StringBuilder* ptr = this;
			ReachEndOfStringBuilderChain(ptr);
			long offset = ptr->_Length;
			Expand(length);

			while (true)
			{
				// Write content of $str to several chunks.
				// Check if space remains is capable for new data.
				if (length < ptr->_Capacity - offset)
				{
					// There is enough space for new data, write them and return.
					Buffer::Wcscpy(ptr->_Data.GetNativePointer() + offset, c_str, length);
					length = 0;
				}
				else
				{
					// There is not enough space in this node, write a part and then move to next node.
					Buffer::Wcscpy(ptr->_Data.GetNativePointer() + offset, c_str, ptr->_Capacity - offset);
					length -= ptr->_Capacity - offset;
				}

				if (length > 0)
				{
					if (ptr->_Next == nullptr)
						break;
					ptr = ptr->_Next;
					offset = 0; // We will not use this anymore.
				}
				else break;
			}
			return *this;
		}
		StringBuilder& StringBuilder::AppendLine(_L_Char c)
		{
			Append(c);
			Append(_LT('\n'));
			return *this;
		}
		StringBuilder& StringBuilder::AppendLine(signed char value)
		{
			return AppendLine(String::FromValue(value));
		}
		StringBuilder& StringBuilder::AppendLine(unsigned char value)
		{
			return AppendLine(String::FromValue(value));
		}
		StringBuilder& StringBuilder::AppendLine(short value)
		{
			return AppendLine(String::FromValue(value));
		}
		StringBuilder& StringBuilder::AppendLine(unsigned short value)
		{
			return AppendLine(String::FromValue(value));
		}
		StringBuilder& StringBuilder::AppendLine(int value)
		{
			return AppendLine(String::FromValue(value));
		}
		StringBuilder& StringBuilder::AppendLine(unsigned int value)
		{
			return AppendLine(String::FromValue(value));
		}
		StringBuilder& StringBuilder::AppendLine(long value)
		{
			return AppendLine(String::FromValue(value));
		}
		StringBuilder& StringBuilder::AppendLine(unsigned long value)
		{
			return AppendLine(String::FromValue(value));
		}
		StringBuilder& StringBuilder::AppendLine(long long value)
		{
			return AppendLine(String::FromValue(value));
		}
		StringBuilder& StringBuilder::AppendLine(unsigned long long value)
		{
			return AppendLine(String::FromValue(value));
		}
		StringBuilder& StringBuilder::AppendLine(String& str)
		{
			Append(str);
			Append(_LT('\n'));
			return *this;
		}
		template<typename ... Args>
		StringBuilder& StringBuilder::AppendFormat(String& format, Args& ... args)
		{

		}
		StringBuilder& StringBuilder::Insert(long index, String& str)
		{
			throw NotImplementedException();
		}
		StringBuilder& StringBuilder::Remove(long index, long length)
		{
			throw NotImplementedException();
		}
		StringBuilder& StringBuilder::Replace(_L_Char oldValue, _L_Char newValue)
		{
			throw NotImplementedException();
		}
		StringBuilder& StringBuilder::Replace(_L_Char oldValue, _L_Char newValue, long from, long count)
		{
			throw NotImplementedException();
		}
		StringBuilder& StringBuilder::Replace(String& oldValue, String& newValue)
		{
			throw NotImplementedException();
		}
		StringBuilder& StringBuilder::Replace(String& oldValue, String& newValue, long from, long count)
		{
			throw NotImplementedException();
		}
		String StringBuilder::ToString()
		{
			StringBuilder* ptr = this;
			long length = 1; // Keep space for '\0'.
			// Calculate the length of return string.
			while (true)
			{
				length += ptr->_Length;
				if (ptr->_Next)
					ptr = ptr->_Next;
				else break;
			}
			_L_Char* c_str = new _L_Char[length];
			// Transfer data.
			ptr = this;
			length = 0;
			while (true)
			{
				Buffer::Wcscpy(c_str + length, ptr->_Data.GetNativePointer(), ptr->_Length);
				length += ptr->_Length;
				if (ptr->_Next)
					ptr = ptr->_Next;
				else break;
			}
			c_str[length] = Char::EndOfString;

			return String(c_str, length + 1);
		}

		// Private

		StringBuilder::StringBuilder(const _L_Char* ptr, long length)
			: _Next(nullptr)
			, _Counter(new ReferenceCounter())
			, _Capacity(_InitialCapacity)
			, _Length(length)
			, _Data()
		{
			_Counter->Inc();
			if (length > _MaxCapacity)
			{
				_Capacity = _MaxCapacity;
				_Data = Array<_L_Char>(_Capacity);
				_Length = _MaxCapacity;
				_Next = new StringBuilder(ptr + _MaxCapacity, length - _MaxCapacity);
				Buffer::Wcscpy(_Data.GetNativePointer(), ptr, _MaxCapacity);
			}
			else if (length < _InitialCapacity)
			{
				_Data = Array<_L_Char>(_InitialCapacity);
				Buffer::Wcscpy(_Data.GetNativePointer(), ptr, length);
			}
			else
			{
				_Capacity = length;
				_Data = Array<_L_Char>(_Capacity);
				Buffer::Wcscpy(_Data.GetNativePointer(), ptr, _Capacity);
			}
		}

		void StringBuilder::Expand(long length)
		{
			StringBuilder* ptr = this;
			ReachEndOfStringBuilderChain(ptr);

			long totalLength = ptr->_Length + length;
			// Check if expansion or reallocation is necessary.
			if (totalLength > ptr->_Capacity)
			{
				// Expansion is required.
				if (totalLength > _MaxCapacity)
				{
					// -1 to prevent the situation that the total number is a multiple of 8192 (and a useless node will be created).
					// +1 to provide a extra node to spread remainders.
					long subobjectCounter = ((totalLength - 1) >> _RShFrom8192To1) + 1;
					while (subobjectCounter-- > 0)
					{
						ptr->_Capacity = _MaxCapacity;
						Array<_L_Char> expanded(_MaxCapacity);
						Array<_L_Char>::Copy(ptr->_Data, expanded, ptr->_Capacity);
						ptr->_Data = expanded;
						ptr->_Next = new StringBuilder();
						ptr = ptr->_Next;
					}
					ptr->Expand(_RemainderOf8192Devision & totalLength);
				}
				// Realocation is required and the current node is capable to store new data.
				else if (totalLength > _InitialCapacity)
				{
					while (totalLength > ptr->_Capacity)
						ptr->_Capacity *= 2;
					if (ptr->_Capacity > _MaxCapacity)
						ptr->_Capacity = _MaxCapacity;

					Array<_L_Char> expanded(_Capacity);
					Array<_L_Char>::Copy(ptr->_Data, expanded, ptr->_Length);
					ptr->_Data = expanded;
				}
			}
			ptr->_Length = totalLength;
		}

		void StringBuilder::ReachEndOfStringBuilderChain(StringBuilder*& ptr)
		{
			while (ptr->_Next)
				ptr = ptr->_Next;
		}
	}
}