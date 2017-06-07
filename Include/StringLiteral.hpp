#pragma once
#include <utility>
#include "_"

_L_NS_BEG

namespace StringLiteral
{
	template<char ... TChars>
	using StringLiteral = std::integer_sequence<char, TChars ...>;

	template<typename TA, typename TB>
	struct Concat
	{
		typedef nullptr_t result;
	};
	template<char ... TCharsA, char ... TCharsB>
	struct Concat<StringLiteral<TCharsA ...>, StringLiteral<TCharsB ...>>
	{
		typedef StringLiteral<TCharsA ..., TCharsB ...> result;
	};

	template<typename, char TC>
	struct Append
	{
		typedef nullptr_t result;
	};
	template<char ... TChars, char TC>
	struct Append<StringLiteral<TChars ...>, TC>
	{
		typedef StringLiteral<TChars ..., TC> result;
	};
}

_L_NS_END

#define _L_DefineStringLiteral(name, str) \
	template<typename T> struct name##Impl; \
	template<size_t ... TIs> struct name##Impl<std::index_sequence<TIs ...>> \
	{ typedef LiongPlus::StringLiteral::StringLiteral<"str"[TIs]...> literal; }; \
	using name = name##Impl<std::make_index_sequence<sizeof("str") - 1>>::literal
