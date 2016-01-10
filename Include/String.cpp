// File: String.cpp
// Author: Rendong Liang (Liong)
#include "String.hpp"
#include "Text/StringBuilder.hpp"

using namespace LiongPlus::Text;

namespace LiongPlus
{
	// Public

	String::String()
		: _Length(1)
		, _Field(new _L_Char[1])
		, _Counter(new ReferenceCounter())
	{
		_Counter->Inc();
		*_Field = Char::EndOfString;
		Chars._OperatorBracket = Func<_L_Char, int>(*this, &String::GetValue);
	}
	String::String(const String& instance)
		: _Length(instance._Length)
		, _Field(instance._Field)
		, _Counter(instance._Counter)
	{
		_Counter->Inc();
		Chars._OperatorBracket = Func<_L_Char, int>(*this, &String::GetValue);
	}
	String::String(String&& instance)
		: _Length(0)
		, _Field(nullptr)
		, _Counter(nullptr)
	{
		Swap(_Length, instance._Length);
		Swap(_Field, instance._Field);
		Swap(_Counter, instance._Counter);
		Chars._OperatorBracket = Func<_L_Char, int>(*this, &String::GetValue);
	}
	String::String(const _L_Char* c_str)
		: _Length(Buffer::Wcslen(c_str))
		, _Field(new _L_Char[_Length])
		, _Counter(new ReferenceCounter())
	{
		Buffer::Wcscpy(_Field, c_str, _Length);

		if (_Counter != nullptr)
			_Counter->Inc();
		Chars._OperatorBracket = Func<_L_Char, int>(*this, &String::GetValue);
	}
	String::String(_L_Char* field, int length)
		: _Length(length)
		, _Field(field)
		, _Counter(new ReferenceCounter())
	{
		_Counter->Inc();
		Chars._OperatorBracket = Func<_L_Char, int>(*this, &String::GetValue);
	}
	String::String(Array<_L_Char>& arr)
		: _Length(arr.GetLength())
		, _Field(new _L_Char[arr.GetLength()])
		, _Counter(new ReferenceCounter())
	{
		_Counter->Inc();
		Chars._OperatorBracket = Func<_L_Char, int>(*this, &String::GetValue);
	}
	String::~String()
	{
		CleanUp();
	}
	String& String::operator=(const String& instance)
	{
		CleanUp();

		_Length = instance._Length;
		_Field = instance._Field;
		_Counter = instance._Counter;
		if (_Counter != nullptr)
			_Counter->Inc();
		Buffer::Wcscpy(_Field, instance._Field, _Length);

		return *this;
	}
	String& String::operator=(String&& instance)
	{
		CleanUp();

		Swap(_Length, instance._Length);
		Swap(_Field, instance._Field);
		Swap(_Counter, instance._Counter);

		return *this;
	}
	String& String::operator=(const _L_Char* c_str)
	{
		CleanUp();

		_Length = Buffer::Wcslen(c_str);
		_Field = new _L_Char[_Length];
		Buffer::Wcscpy(_Field, c_str, _Length);
		_Field[_Length] = 0x00;
		_Counter = new ReferenceCounter();
		_Counter->Inc();

		return *this;
	}
	String& String::operator=(Array<_L_Char>& arr)
	{
		CleanUp();

		_Length = arr.GetLength();
		_Field = new _L_Char[_Length];
		Buffer::Wcscpy(_Field, arr.GetNativePointer(), _Length);
		--_Length;
		_Counter = new ReferenceCounter();
		_Counter->Inc();
		return *this;
	}

	bool String::operator==(String& str) const
	{
		if (_Length == str._Length)
			return wcscmp(_Field, str._Field) == 0;
		else
			return false;
	}
	bool String::operator!=(String& str) const
	{
		if (_Length == str._Length)
			return wcscmp(_Field, str._Field) != 0;
		else
			return true;
	}
	String& String::operator+=(String& str)
	{
		*this = Concat(*this, str);
		return *this;
	}
	String& String::operator+=(const _L_Char* str)
	{
		*this = Concat(*this, String(str));
		return *this;
	}
	String& String::operator+=(const int str)
	{
		*this = Concat(*this, FromValue(str));
		return *this;
	}
	String& String::operator+=(const double str)
	{
		*this = Concat(*this, FromValue(str));
		return *this;
	}
	String& String::operator+=(const float str)
	{
		*this = Concat(*this, FromValue(str));
		return *this;
	}
	String String::operator+(String& str)
	{
		return Concat(*this, str);
	}
	String String::operator+(const _L_Char* str)
	{
		return Concat(*this, String(str));
	}
	String String::operator+(const int str)
	{
		return Concat(*this, FromValue(str));
	}
	String String::operator+(const double str)
	{
		return Concat(*this, FromValue(str));
	}
	String String::operator+(const float str)
	{
		return Concat(*this, FromValue(str));
	}

	EnumeratorParser<_L_Char> String::begin()
	{
		return new TEnumerator(_Field, _Length, 0);
	}
	EnumeratorParser<_L_Char> String::end()
	{
		return new TEnumerator(_Field, _Length, _Length - 1);
	}

	Ptr<String> String::Clone()
	{
		return new String(_Field);
	}

	int String::CompareTo(String& value)
	{
		return Compare(*this, value);
	}

	bool String::Contains(String& value)
	{
		if (_Length < value._Length)
			return false; // A string can never contain a string which is inter than itself.

		int toDo = _Length - value._Length;
		while (--toDo >= 0)
		{
			if (CompareSection(_Field, value._Field, value._Length) == 0)
				return true;
		}

		return false;
	}

	bool String::Equals(String& value)
	{
		return wcscmp(_Field, value._Field) == 0;
	}

	const _L_Char* String::GetNativePointer()
	{
		return _Field;
	}

	int String::GetLength()
	{
		return _Length;
	}
    
    String String::Remove(int index)
    {
        _L_Char* c_str = new _L_Char[index + 1];
        Buffer::Wcscpy(c_str, _Field, index);
        c_str[index] = Char::EndOfString;
        return String(c_str, index + 1);
    }
    String String::Remove(int index, int count)
    {
        _L_Char* c_str = new _L_Char[_Length - count];
        Buffer::Wcscpy(c_str, _Field, index);
        Buffer::Wcscpy(c_str + index, _Field + index + count, _Length - index - count);
        return String(c_str, _Length - count);
    }
    
	String& String::ToString()
	{
		return *this;
	}

	// Static

	int String::Compare(String& a, String& b)
	{
		int difference = CompareSection(a._Field, b._Field, a._Length < b._Length ? a._Length : b._Length);

		if (difference == 0)
			return a._Length == b._Length ? 0 : a._Length - b._Length;
		else
			return difference;
	}

	String String::Concat(String& str1, String& str2)
	{
		int size = str1._Length + str2._Length - 1;
		_L_Char* c_str = new _L_Char[size];
		Buffer::Wcscpy(c_str, str1._Field, str1._Length - 1);
		Buffer::Wcscpy(c_str + str1._Length - 1, str2._Field, str2._Length);
		
		return String(c_str, size);
	}
	String String::Concat(std::initializer_list<String> strs)
	{
		int size = 0;
		for (auto& str : strs)
		{
			size += str._Length - 1;
		}
		_L_Char* c_str = new _L_Char[size + 1];
		size = 0;
		for (auto& str : strs)
		{
			Buffer::Wcscpy(c_str + size, str._Field, str._Length - 1);
			size += str._Length - 1;
		}
		c_str[size] = Char::EndOfString; // 'NUL'.

		return String(c_str, size + 1);
	}
	String String::Concat(Array<String> strs)
	{
		int length = 0;
		for (auto& str : strs)
			length += str._Length- 1;
		_L_Char* c_str = new _L_Char[length + 1];
		length = 0;
		for (auto& str : strs)
		{
			Buffer::Wcscpy(c_str + length, str._Field, str._Length - 1);
			length += str._Length - 1;
		}
		c_str[length] = Char::EndOfString; // 'NUL'.

		return String(c_str, length + 1);
	}

	String String::FromValue(unsigned long long value)
	{
		return FromUnsignedInterger(value);
	}
	String String::FromValue(long long value)
	{
		return FromSignedInterger(value);
	}
	String String::FromValue(unsigned long value)
	{
		return FromUnsignedInterger(value);
	}
	String String::FromValue(long value)
	{
		return FromSignedInterger(value);
	}
	String String::FromValue(unsigned int value)
	{
		return FromUnsignedInterger(value);
	}
	String String::FromValue(int value)
	{
		return FromSignedInterger(value);
	}
	String String::FromValue(unsigned short value)
	{
		return FromUnsignedInterger(value);
	}
	String String::FromValue(short value)
	{
		return FromSignedInterger(value);
	}
	String String::FromValue(unsigned char value)
	{
		return FromUnsignedInterger(value);
	}
	String String::FromValue(char value)
	{
		return FromSignedInterger(value);
	}
	String String::FromValue(double value)
	{
		return FromFloatingPoint(value);
	}
	String String::FromValue(float value)
	{
		return FromFloatingPoint(value);
	}
	String String::FromValue(bool value)
	{
		return String(value ? _LT("true") : _LT("false"));
	}

	String String::Join(String& separator, Array<String>& values, int index, int count)
	{
		if (index < 0 || count < 0)
			throw ArgumentOutOfRangeException("Need non-negative number.");
		if (index + count > values.GetLength())
			throw ArgumentOutOfRangeException("Bound exceeded.");
		if (count == 0)
			return String::Empty;
		
		StringBuilder str;
		--count;
		for (int i = 0; i < count; ++i)
		{
			str.Append(values[index + i]);
			str.Append(separator);
		}
		str.Append(values[index + count]);

		return str.ToString();
	}
	String String::Join(String& separator, Array<String>& values)
	{
		return Join(separator, values, 0, values.GetLength());
	}
    
    

	// IBuffer

	const Byte* String::AbandonBuffer()
	{
		const Byte* ptr = reinterpret_cast<const Byte*>(_Field);
		_Field = new _L_Char[_Length + 1];
		_Field[_Length + 1] = 1;
		return ptr;
	}

	const Byte* String::AccessBuffer()
	{
		return reinterpret_cast<const Byte*>(_Field);
	}

	bool String::IsBufferAccessable()
	{
		return true;
	}

	// IEnumerable<T>

	Ptr<IEnumerator<_L_Char>> String::GetEnumerator()
	{
		return new TEnumerator(_Field, _Length, 0);
	}

	String String::Empty = String();

	// Private

	int String::CompareSection(_L_Char* a, _L_Char* b, int length)
	{
		while (--length >= 0)
		{
			if (a[length] == 0 || b[length] == 0)
			{
				if ((a[length] - b[length]) != 0)
					return (a[length] - b[length]);
			}
		}
		return 0;
	}

	_L_Char String::GetValue(int index)
	{
		return *(_Field + index);
	}

	void String::CleanUp()
	{
		_Length = 0;
		if (_Counter && !_Counter->Dec())
		{
			if (_Field)
			{
				delete[] _Field;
			}
			delete _Counter;
		}
		_Field = nullptr;
		_Counter = nullptr;
	}
}