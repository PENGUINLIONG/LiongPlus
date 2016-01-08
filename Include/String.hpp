// File: String.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_String
#define _L_String
#include "Fundamental.hpp"
#include "Array.hpp"
#include "Buffer.hpp"
#include "Char.hpp"
#include "Collections\ContinuousMemoryEnumerator.hpp"
#include "Delegate.hpp"
#include "Exception.hpp"
#include "IStandardizable.hpp"
#include "Ptr.hpp"
#include "ReferenceCounter.hpp"

using namespace LiongPlus::Collections;

namespace LiongPlus
{
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

		struct PropertyChars
		{
			friend class String;
		private:
			Func<_L_Char, int> _OperatorBracket;
		public:
			_L_Char operator[](int index)
			{
				return _OperatorBracket(index);
			}
		};

		PropertyChars Chars;

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
		String& ToString();

		static int Compare(String& a, String& b);
		static String Concat(String& str1, String& str2);
		static String Concat(std::initializer_list<String> strs);
		static String Concat(Array<String> strs);
		template<typename ... Args>
		static String Format(String format, Args ... args)
		{
			return format;
		}
		/// <summary>
		/// Write the numeric content of $value to a string.
		/// </summary>
		/// <param name="value">Number to be Processed.</param>
		/// <param name="string">A string.</param>
		/// <returns>A string containing the numeric content of $value.</returns>
		static String FromValue(unsigned int value);
		/// <summary>
		/// Write the numeric content of $value to a string.
		/// </summary>
		/// <param name="value">Number to be Processed.</param>
		/// <param name="string">A string.</param>
		/// <returns>A string containing the numeric content of $value.</returns>
		static String FromValue(int value);
		/// <summary>
		/// Write the numeric content of $value to a string.
		/// </summary>
		/// <param name="value">Number to be Processed.</param>
		/// <param name="string">A string.</param>
		/// <returns>A string containing the numeric content of $value.</returns>
		/// <remarks>Output string may be inaccurate because of algorithm. Especially with a int fraction part.</remarks>
		static String FromValue(double value);
		/// <summary>
		/// Write the numeric content of $value to a string.
		/// </summary>
		/// <param name="value">Number to be Processed.</param>
		/// <param name="string">A string.</param>
		/// <returns>A string containing the numeric content of $value.</returns>
		/// <remarks>Output string may be inaccurate because of algorithm. Especially with a int fraction part.</remarks>
		static String FromValue(float value);
		/// <summary>
		/// Write the boolean content of $value to a string.
		/// </summary>
		/// <param name="value">Number to be Processed.</param>
		/// <param name="string">A string.</param>
		/// <returns>Count of significant digits (and negative sign).</returns>
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
		static int FractionWcsLength(double floating);

		/// <summary>
		/// Count the length of $ininterger which can be used in requesting dynamic memories.
		/// </summary>
		/// <param name="interger">the interger to be counted.</param>
		/// <returns>Count of chars, 'NUL' will be included.</returns>
		static int IntergerWcsLength(int interger);

		/// <summary>
		/// Count the length of $ininterger which can be used in requesting dynamic memories.
		/// </summary>
		/// <param name="interger">the interger to be counted.</param>
		/// <returns>Count of chars, 'NUL' will be included.</returns>
		static int UnsignedIntergerWcsLength(unsigned int interger);

		virtual void CleanUp();
	};
}
#endif