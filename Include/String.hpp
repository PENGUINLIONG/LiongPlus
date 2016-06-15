// File: String.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_String
#define _L_String
#include "Fundamental.hpp"
#include "IO/IBuffer.hpp"
#include "Buffer.hpp"
#include "Char.hpp"
#include "Collections/ContinuousMemoryEnumerator.hpp"
#include "Collections/List.hpp"
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
		String(_L_Char* field, long length);
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
		String& operator+=(const long str);
		String& operator+=(const double str);
		String& operator+=(const float str);
		String operator+(String& str);
		String operator+(const _L_Char* str);
		String operator+(const long str);
		String operator+(const double str);
		String operator+(const float str);
		_L_Char operator[](long index);

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
		long CompareTo(String& value);
		bool Contains(String& value);
		bool Equals(String& value);
		const _L_Char* GetNativePointer();
		long GetLength();
		String Insert(long index, String& value);
		bool IsNumber();
		String Remove(long index);
		String Remove(long index, long count);
		Array<String> Split(_L_Char separator, StringSplitOptions option = StringSplitOptions::RemoveEmptyEntries);
		Array<String> Split(_L_Char separator, long maxCount, StringSplitOptions option = StringSplitOptions::RemoveEmptyEntries);
		Array<String> Split(Array<_L_Char>& separators, StringSplitOptions option = StringSplitOptions::RemoveEmptyEntries);
		Array<String> Split(Array<_L_Char>& separators, long maxCount, StringSplitOptions option = StringSplitOptions::RemoveEmptyEntries);
		//Array<String> Split(std::initializer_list<_L_Char> separators, StringSplitOptions option);
		//Array<String> Split(std::initializer_list<_L_Char> separators, long maxCount, StringSplitOptions option = SplitOptions::RemoveEmptyEntries);
		String Substring(long index);
		String Substring(long index, long count);
		String& ToString();
		String Trim();
		String Trim(Array<_L_Char>& trimee);
		String TrimEnd(Array<_L_Char>& trimee);
		String TrimStart(Array<_L_Char>& trimee);

		static long Compare(String& a, String& b);
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
		static String FromValue(signed char value);
		static String FromValue(double value);
		static String FromValue(float value);
		static String FromValue(bool value);

		static String Join(String& separator, Array<String>& values, long index, long count);
		static String Join(String& separator, Array<String>& values);

		static unsigned long long ToUlonglong(String& str) { return ToNumber<unsigned long long>(str); }
		static long long ToLonglong(String& str) { return ToNumber<long long>(str); }
		static unsigned long ToUlong(String& str) { return ToNumber<unsigned long>(str); }
		static long ToLong(String& str) { return ToNumber<long>(str); }
		static unsigned int ToUint(String& str) { return ToNumber<unsigned int>(str); }
		static int ToInt(String& str) { return ToNumber<int>(str); }
		static unsigned short ToUshort(String& str) { return ToNumber<unsigned short>(str); }
		static short ToShort(String& str) { return ToNumber<short>(str); }
		static unsigned char ToUchar(String& str) { return ToNumber<unsigned char>(str); }
		static signed char ToChar(String& str) { return ToNumber<signed char>(str); }
		static double ToDouble(String& str) { return ToNumber<double>(str); }
		static float ToFloat(String& str) { return ToNumber<float>(str); }

		// IBuffer

		virtual Byte* AbandonBuffer();
		virtual const Byte* AccessBuffer();
		virtual bool IsBufferAccessable();

		// IEnumerable<T>

		virtual Ptr<IEnumerator<_L_Char>> GetEnumerator() override final;

	private:
		long _Length;
		_L_Char* _Field;

		typedef ContinuousMemoryEnumerator<_L_Char> TEnumerator;

		_L_Char GetValue(long index);

		static long CompareSection(_L_Char* a, _L_Char* b, long length);

		/// <summary>
		/// Count the length of $floating which can be used in requesting dynamic memories.
		/// </summary>
		/// <param name="floating">the interger to be counted.</param>
		/// <returns>Count of chars, 'NUL' will be included.</returns>
		template<typename T>
		static long FractionWcsLength(T floating)
		{
			long len = 2; // '.' and 'NUL'.
			floating -= (long)floating;

			if (floating == 0)
				return 0;
			if (floating < 0)
				floating = -floating;
			do
			{
				++len;
				floating *= 10;
				floating -= (long)floating;
			} while (floating - 0.000001 > 0); // For the low accuracy of floating point number culculation.

			return len;
		}

		/// <summary>
		/// Count the length of $ininterger which can be used in requesting dynamic memories.
		/// </summary>
		/// <param name="interger">the interger to be counted.</param>
		/// <returns>Count of chars, 'NUL' will be included.</returns>
		template<typename T>
		static long IntergerWcsLength(T interger)
		{
			if (interger == 0)
				return 2;
			long len = 1;
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
		static long UnsignedIntergerWcsLength(T interger)
		{
			if (interger < 10)
				return 2;

			long len = 1;
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

			long len = UnsignedIntergerWcsLength(value);
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

			long len = IntergerWcsLength(value);
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

			long len = FractionWcsLength(value);
			String interger, fraction = String(new _L_Char[len], len);
			if ((long)value == 0 && value < -0.000001)
			{
				_L_Char* c_str = new _L_Char[3];
				c_str[0] = _LT('-');
				c_str[1] = _LT('0');
				c_str[2] = Char::EndOfString;
				interger = String(c_str, 3);
			}
			else
				interger = FromValue((long)value);
			_L_Char* c_str = fraction._Field;
			if (value < 0.0)
				value = -value;
			value -= (long)value;

			*(c_str) = _LT('.'); // Decimal point.

			do
			{
				value *= 10;
				*(++c_str) = (wchar_t)value + 0x30;
				value -= (long)value;
			} while (value - 0.000001 > 0); // For the low accuracy of floating point number culculation.

			*(++c_str) = 0; // 'NUL'.

			return interger + fraction;
		}

		template<typename T>
		static T ToNumber(String& str)
		{
			if (str._Length <= 1)
				return false;
			const _L_Char* beg = str._Field;
			const _L_Char* end = beg + str._Length - 1;

			TrimRange(beg, end);
			// Now, there should not be any ' ' in range.
			if (*beg == _LT('e') || *end == _LT('e'))
				return false;

			// Split exponential part.
			const _L_Char* ePos = beg;
			const _L_Char* dotPos = nullptr;
			while (*(++ePos) != _LT('e'))
			{
				if (*ePos == _LT('.'))
					dotPos = ePos;
				if (ePos == end && IsNumberImpl(beg, end))
				{
					if (dotPos == nullptr)
						return (T)ToInteger(beg, end);
					else
					{
						T dec = (T)ToInteger(dotPos + 1, end);
						return (T)ToInteger(beg, dotPos - 1) + dec / (T)(IntergerWcsLength(dec) - 1);
					}
				}
			}

			if (ePos != end - 1)
			{
				if (IsNumberImpl(beg, ePos - 1) && IsNumberImplE(ePos + 1, end))
				{
					T dec = (T)ToInteger(dotPos + 1, ePos - 1);
					if (dotPos != nullptr)
						return std::pow((T)ToInteger(beg, dotPos - 1) + dec / (T)(IntergerWcsLength(dec) - 1), (T)ToInteger(ePos + 1, end));
					else
						return std::pow((T)ToInteger(beg, ePos - 1), (T)ToInteger(ePos + 1, end));
				}
			}
			throw ArgumentException("Not a number string.($str)");
		}

		inline static bool IsNumberImpl(const _L_Char* beg, const _L_Char* end)
		{
			long dotCount = 0;

			// Omit signs.
			if (*beg == _LT('-') || *beg == _LT('+'))
				++beg;

			// Check the first char in range.
			// If the first char is not a numeric char or a dot, the string is absolutely not a number.
			if (Char::IsDigit(*beg));
			else if (*beg == '.' && beg < end) // If the dot is the only char in range, the string is not a number.
				++dotCount;
			else return false;

			while (++beg <= end)
			{
				if (Char::IsDigit(*beg));
				else if (*beg == _LT('.'))
					++dotCount;
				else
					break;
			}
			return beg > end && dotCount <= 1;
		}
		
		inline static bool IsNumberImplE(const _L_Char* beg, const _L_Char* end)
		{
			if (*beg == _LT('-') || *beg == _LT('+'))
				++beg;

			while (Char::IsDigit(*beg))
			{
				if (++beg > end)
					return true;
			}
			return false;
		}

		inline static long ToInteger(const _L_Char* beg, const _L_Char* end)
		{
			bool neg = false;
			long rv = 0;
			if (*beg == _LT('+'))
				++beg;
			if (*beg == _LT('-'))
			{
				++beg;
				neg = true;
			}

			while (beg <= end)
			{
				rv += *beg - _LT('0');
				rv *= 10;
				++beg;
			}
			return neg ? rv * -1 : rv;
		}
		
		inline static void TrimRange(const _L_Char*& beg, const _L_Char*& end)
		{
			while ((*(beg++) == _LT(' ') || *beg == _LT('\r') || *beg == _LT('\n')) && beg <= end);
			--beg;
			while ((*(--end) == _LT(' ') || *end == _LT('\r') || *end == _LT('\n')) && end >= beg); // --end, because the end is at '\0'.
			++end;
		}

		void CleanUp();
	};
}
#endif