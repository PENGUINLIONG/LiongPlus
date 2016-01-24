// File: StringFormatter.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_StringFormatter
#define _L_StringFormatter
#include "../Fundamental.hpp"
#include "../Collections/List.hpp"
#include "../String.hpp"
#include "StringBuilder.hpp"

namespace LiongPlus
{
	namespace Text
	{
		/*
		* To use CTFormatter like this:
		* CTFormatter<String, int, _L_Char> formatter;
		* formatter.ToString(_LT("Hi! I'm agent"), 12450, _LT('!'));
		*/
		
		/// <summary>
		/// A template-based string formatter. CT for Compile time as the insertion 'sites' are decided in compile time.
		/// </summary>
		/// <typeparam name="TArgs">The type list of insertion values.</typeparam>
		/// <note>This is suitable for formatting with a few and non-repeated params. If not so, please use [LiongPlus::Text::RTFormatter] instead.</note>
		template<typename ... TArgs>
		class CTFormatter
			: public Object
		{
		public:
			String ToString(TArgs&& ... args)
			{
				StringBuilder builder;
				int callerArr[] = { 0, ((void)builder.Append(Forward<TArgs>(args)), 0) ... }; // Only the expressions using param pack in initializer list can be executed(compiled). As an optimization, this array will be omitted by compiler generally. The second 0 provides values of type [long] that satisfies the type of $callerArr. For more detail about the second 0, check out comma operator.

				return builder.ToString();
			}

			/* [C++17 Version]
			String ToString(TArgs&& ... args)
			{
				StringBuilder builder;
				(ImportParam(builder, builder.Append(args)), ...); // Fold expression

				return builder.ToString();
			}
			*/
		};
	}
}
#endif