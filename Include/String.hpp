// File: String.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_String
#define _L_String
#include "Fundamental.hpp"
#include "Array.hpp"
#include "Buffer.hpp"
#include "Char.hpp"
#include "Collections\ContinuousMemoryEnumerator.hpp"
#include "Collections\List.hpp"
#include "Delegate.hpp"
#include "Exception.hpp"
#include "Ptr.hpp"
#include "ReferenceCounter.hpp"

using namespace LiongPlus::Collections;

namespace LiongPlus
{
	enum class StringSplitOptions
	{
		None,
		RemoveEmptyEntries
	};
	
	/// <summary>
	/// Represents text as a series of characters.
	/// </summary>
	/// <remarks>LiongPlus will use [_L_Char] (16-bit) for less complexity. To convert string data into other encoding, see [LiongPlus::Encoding]</remarks>
	class String
		: public Object
		, public IBuffer
		, public IEnumerable<_L_Char>
	{
	public:
		static String Empty;

		String();
		String(const String& instance);
		String(String&& instance);
		String(const _L_Char* c_str);
		String(_L_Char* field, int length);
		String(Array<_L_Char>& arr);
		~String();

		String& operator=(const String& instance);
		String& operator=(String&& instance);
		String& operator=(const _L_Char* c_str);
		String& operator=(Array<_L_Char>& arr);

		bool operator==(String& str) const;
		bool operator!=(String& str) const;
		String& operator+=(String& str);
		String& operator+=(const _L_Char* str);
		String& operator+=(const int str);
		String& operator+=(const double str);
		String& operator+=(const float str);
		String operator+(String& str);
		String operator+(const _L_Char* str);
		String operator+(const int str);
		String operator+(const double str);
		String operator+(const float str);

		/// <summary>
		/// Take the advantage of C++11 range-based loop. Never call this method, please call GetEnumerator for [LiongPlus::Collections::IEnumerator].
		/// </summary>
		/// <returns>Parsed [LiongPlus::Collections::IEnumerator] which at the end of the collection.</returns>
		EnumeratorParser<_L_Char> begin();
		/// <summary>
		/// Take the advantage of C++11 range-based loop. Never call this method, please call GetEnumerator for [LiongPlus::Collections::IEnumerator].
		/// </summary>
		/// <returns>Parsed [LiongPlus::Collections::IEnumerator] which at the end of the collection.</returns>
		EnumeratorParser<_L_Char> end();

		Ptr<String> Clone();
		int CompareTo(String& value);
		bool Contains(String& value);
		bool Equals(String& value);
		const _L_Char* GetNativePointer();
		int GetLength();
		String Insert(int index, String value);
		String Remove(int index);
		String Remove(int index, int count);
		Array<String> Split(_L_Char separator, StringSplitOptions option = StringSplitOptions::RemoveEmptyEntries);
		Array<String> Split(_L_Char separator, int maxCount, StringSplitOptions option = StringSplitOptions::RemoveEmptyEntries);
		Array<String> Split(Array<_L_Char>& separators, StringSplitOptions option);
		Array<String> Split(Array<_L_Char>& separators, int maxCount, StringSplitOptions option = StringSplitOptions::RemoveEmptyEntries);
		//Array<String> Split(std::initializer_list<_L_Char> separators, StringSplitOptions option);
		//Array<String> Split(std::initializer_list<_L_Char> separators, int maxCount, StringSplitOptions option = SplitOptions::RemoveEmptyEntries);
		String Substring(int index);
		String Substring(int index, int count);
		String& ToString();
		String Trim();
		String Trim(Array<_L_Char>& trimee);
		String TrimEnd(Array<_L_Char>& trimee);
		String TrimStart(Array<_L_Char>& trimee);

		static int Compare(String& a, String& b);
		static String Concat(String& str1, String& str2);
		static String Concat(std::initializer_list<String> strs);
		static String Concat(Array<String> strs);
		template<typename ... Args>
		static String Format(String& format, Args ... args)
		{
			return format;
		}

		static String FromValue(unsigned long long value);
		static String FromValue(long long value);
		static String FromValue(unsigned long value);
		static String FromValue(long value);
		static String FromValue(unsigned int value);
		static String FromValue(int value);
		static String FromValue(unsigned short value);
		static String FromValue(short value);
		static String FromValue(unsigned char value);
		static String FromValue(char value);
		static String FromValue(double value);
		static String FromValue(float value);
		static String FromValue(bool value);

		static String Join(String& separator, Array<String>& values, int index, int count);
		static String Join(String& separator, Array<String>& values);

		// IBuffer

		virtual const Byte* AbandonBuffer();
		virtual const Byte* AccessBuffer();
		virtual bool IsBufferAccessable();

		// IEnumerable<T>

		virtual Ptr<IEnumerator<_L_Char>> GetEnumerator() override final;

	private:
		int _Length;
		_L_Char* _Field;
		ReferenceCounter* _Counter;

		typedef ContinuousMemoryEnumerator<_L_Char> TEnumerator;

		_L_Char GetValue(int index);

		static int CompareSection(_L_Char* a, _L_Char* b, int length);

		/// <summary>
		/// Count the length of $floating which can be used in requesting dynamic memories.
		/// </summary>
		/// <param name="floating">the interger to be counted.</param>
		/// <returns>Count of chars, 'NUL' will be included.</returns>
		template<typename T>
		static int FractionWcsLength(T floating)
		{
			int len = 2; // '.' and 'NUL'.
			floating -= (int)floating;

			if (floating == 0)
				return 0;
			if (floating < 0)
				floating = -floating;
			do
			{
				++len;
				floating *= 10;
				floating -= (int)floating;
			} while (floating - 0.000001 > 0); // For the low accuracy of floating point number culculation.

			return len;
		}

		/// <summary>
		/// Count the length of $ininterger which can be used in requesting dynamic memories.
		/// </summary>
		/// <param name="interger">the interger to be counted.</param>
		/// <returns>Count of chars, 'NUL' will be included.</returns>
		template<typename T>
		static int IntergerWcsLength(T interger)
		{
			if (interger == 0)
				return 2;
			int len = 1;
			if (interger < 0)
				++len;

			while (interger != 0)
			{
				++len;
				interger /= 10;
			}

			return len;
		}

		/// <summary>
		/// Count the length of $ininterger which can be used in requesting dynamic memories.
		/// </summary>
		/// <param name="interger">the interger to be counted.</param>
		/// <returns>Count of chars, 'NUL' will be included.</returns>
		template<typename T>
		static int UnsignedIntergerWcsLength(T interger)
		{
			if (interger < 10)
				return 2;

			int len = 1;
			while (interger != 0)
			{
				++len;
				interger /= 10;
			}

			return len;
		}
		
		template<typename T>
		static String FromUnsignedInterger(T value)
		{
			if (value == 0)
			{
				_L_Char* c_str = new _L_Char[2];
				c_str[0] = _LT('0');
				c_str[1] = Char::EndOfString;
				return String(c_str, 2);
			}

			int len = UnsignedIntergerWcsLength(value);
			String output(new _L_Char[len], len);
			_L_Char* beg, *end;

			beg = end = output._Field;

			*end = 0; // 'NUL'.

					  // Write.
			while (value != 0)
			{
				*(++end) = (value % 10) + 0x30; // Keep space for 'NUL'.
				value /= 10;
			}

			// Reverse.
			while (beg < end) // XOR exchange.
			{
				*beg ^= *end;
				*end ^= *beg;
				*(beg++) ^= *(end--);
			}

			return output;

		}

		template<typename T>
		static String FromSignedInterger(T value)
		{
			if (value == 0)
			{
				_L_Char* c_str = new _L_Char[2];
				c_str[0] = _LT('0');
				c_str[1] = Char::EndOfString;
				return String(c_str, 2);
			}

			int len = IntergerWcsLength(value);
			String output(new _L_Char[len], len);
			_L_Char* beg, *end;
			bool neg;

			if (neg = value < 0)
				value = -value;

			beg = end = output._Field;

			*end = 0; // 'NUL'.

			// Write.
			while (value != 0)
			{
				*(++end) = (value % 10) + 0x30; // Keep space for 'NUL'.
				value /= 10;
			}

			// Put negative sign.
			if (neg)
				*(++end) = _LT('-');

			// Reverse.
			while (beg < end) // XOR exchange.
			{
				*beg ^= *end;
				*end ^= *beg;
				*(beg++) ^= *(end--);
			}

			return output;
		}

		template<typename T>
		static String FromFloatingPoint(T value)
		{
			if (value == 0.0)
			{
				_L_Char* c_str = new _L_Char[2];
				c_str[0] = _LT('0');
				c_str[1] = Char::EndOfString;
				return String(c_str, 2);
			}

			int len = FractionWcsLength(value);
			String interger, fraction = String(new _L_Char[len], len);
			if ((int)value == 0 && value < -0.000001)
			{
				_L_Char* c_str = new _L_Char[3];
				c_str[0] = _LT('-');
				c_str[1] = _LT('0');
				c_str[2] = Char::EndOfString;
				interger = String(c_str, 3);
			}
			else
				interger = FromValue((int)value);
			_L_Char* c_str = fraction._Field;
			if (value < 0.0)
				value = -value;
			value -= (int)value;

			*(c_str) = _LT('.'); // Decimal point.

			do
			{
				value *= 10;
				*(++c_str) = (int)value + 0x30;
				value -= (int)value;
			} while (value - 0.000001 > 0); // For the low accuracy of floating point number culculation.

			*(++c_str) = 0; // 'NUL'.

			return interger + fraction;
		}

		virtual void CleanUp();
	};
}
#endif