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
			StringBuilder(int capacity);
			StringBuilder(String& str);
			StringBuilder(const StringBuilder& instance);
			StringBuilder(StringBuilder&& instance);
			StringBuilder(const _L_Char* c_str);
			~StringBuilder();
			
			StringBuilder& operator=(const StringBuilder& instance);
			StringBuilder& operator=(StringBuilder&& instance);

			void Append(_L_Char c);
			void Append(String& str);
			void AppendLine(_L_Char c);
			void AppendLine(String& str);
			template<typename ... Args>
			void AppendFormat(String& format, Args& ... args);
			void Insert(int index, String& string);
			void Remove(int index, int length);
			void Replace(_L_Char oldValue, _L_Char newValue);
			void Replace(_L_Char oldValue, _L_Char newValue, int from, int count);
			void Replace(String& oldValue, String& newValue);
			void Replace(String& oldValue, String& newValue, int from, int count);
			String ToString();
		private:
			class FormatHelper
			{
			public:
				FormatHelper();
				~FormatHelper();

				template<typename T>
				void ImportParam(T value);
				String ToString();
			private:
				int _Accumulator;
				List<String> _Builder;
			};

			StringBuilder* _Next;
			ReferenceCounter* _Counter;
			int _Capacity, _Length;
			Array<_L_Char> _Data;

			const static int _RShFrom8192To1 = 13;
			const static int _RemainderOf8192Devision = 0x1FFF;
			const static int _InitialCapacity = 32;
			const static int _MaxCapacity = 8192;

			StringBuilder(const _L_Char* ptr, int length);
			void Expand(int length);

			void ReachEndOfStringBuilderChain(StringBuilder*& origin);
		};
	}
}
#endif