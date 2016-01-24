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

			void Append(_L_Char c);
			void Append(signed char value);
			void Append(unsigned char value);
			void Append(signed short value);
			void Append(unsigned short value);
			void Append(signed int value);
			void Append(unsigned int value);
			void Append(signed long value);
			void Append(unsigned long value);
			void Append(signed long long value);
			void Append(unsigned long long value);
			void Append(String& str);
			void AppendLine(_L_Char c);
			void AppendLine(signed char value);
			void AppendLine(unsigned char value);
			void AppendLine(signed short value);
			void AppendLine(unsigned short value);
			void AppendLine(signed int value);
			void AppendLine(unsigned int value);
			void AppendLine(signed long value);
			void AppendLine(unsigned long value);
			void AppendLine(signed long long value);
			void AppendLine(unsigned long long value);
			void AppendLine(String& str);
			template<typename ... Args>
			void AppendFormat(String& format, Args& ... args);
			void Insert(long index, String& string);
			void Remove(long index, long length);
			void Replace(_L_Char oldValue, _L_Char newValue);
			void Replace(_L_Char oldValue, _L_Char newValue, long from, long count);
			void Replace(String& oldValue, String& newValue);
			void Replace(String& oldValue, String& newValue, long from, long count);
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