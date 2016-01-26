// File: StringBuilder.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_StringBuilder
#define _L_StringBuilder
#include "../Fundamental.hpp"
#include "../Char.hpp"
#include "../Collections/List.hpp"
#include "../String.hpp"
#include "../ReferenceCounter.hpp"

namespace LiongPlus
{
	namespace Text
	{
		class StringBuilder
			: public Object
		{
		public:
			StringBuilder();
			StringBuilder(long capacity);
			StringBuilder(String& str);
			StringBuilder(const StringBuilder& instance);
			StringBuilder(StringBuilder&& instance);
			StringBuilder(const _L_Char* c_str);
			~StringBuilder();
			
			StringBuilder& operator=(const StringBuilder& instance);
			StringBuilder& operator=(StringBuilder&& instance);

			StringBuilder& Append(_L_Char c);
			StringBuilder& Append(signed char value);
			StringBuilder& Append(unsigned char value);
			StringBuilder& Append(short value);
			StringBuilder& Append(unsigned short value);
			StringBuilder& Append(int value);
			StringBuilder& Append(unsigned int value);
			StringBuilder& Append(long value);
			StringBuilder& Append(unsigned long value);
			StringBuilder& Append(long long value);
			StringBuilder& Append(unsigned long long value);
			StringBuilder& Append(String& str);
			StringBuilder& AppendLine(_L_Char c);
			StringBuilder& AppendLine(signed char value);
			StringBuilder& AppendLine(unsigned char value);
			StringBuilder& AppendLine(short value);
			StringBuilder& AppendLine(unsigned short value);
			StringBuilder& AppendLine(int value);
			StringBuilder& AppendLine(unsigned int value);
			StringBuilder& AppendLine(long value);
			StringBuilder& AppendLine(unsigned long value);
			StringBuilder& AppendLine(long long value);
			StringBuilder& AppendLine(unsigned long long value);
			StringBuilder& AppendLine(String& str);
			template<typename ... Args>
			StringBuilder& AppendFormat(String& format, Args& ... args);
			StringBuilder& Insert(long index, String& string);
			StringBuilder& Remove(long index, long length);
			StringBuilder& Replace(_L_Char oldValue, _L_Char newValue);
			StringBuilder& Replace(_L_Char oldValue, _L_Char newValue, long from, long count);
			StringBuilder& Replace(String& oldValue, String& newValue);
			StringBuilder& Replace(String& oldValue, String& newValue, long from, long count);
			String ToString();
		private:
			StringBuilder* _Next;
			ReferenceCounter* _Counter;
			long _Capacity, _Length;
			Array<_L_Char> _Data;

			const static long _RShFrom8192To1 = 13;
			const static long _RemainderOf8192Devision = 0x1FFF;
			const static long _InitialCapacity = 32;
			const static long _MaxCapacity = 8192;

			StringBuilder(const _L_Char* ptr, long length);
			void Expand(long length);

			void ReachEndOfStringBuilderChain(StringBuilder*& origin);
		};
	}
}
#endif