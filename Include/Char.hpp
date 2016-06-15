// File: Char.cpp
// Author: Rendong Liang (Liong)

#ifndef _L_Char_
#define _L_Char_
#include "Fundamental.hpp"
#include "Exception.hpp"

namespace LiongPlus
{
	class Char
	{
	public:
		const static _L_Char NextLine;
		const static _L_Char EndOfString;
		
		// The following methods are designed for ASCII chars. Char code greater than 0x007F are not supported.
		static bool IsControl(_L_Char c);
		static bool IsDigit(_L_Char c);
		static bool IsLetter(_L_Char c);
		static bool IsLetterOrDigit(_L_Char c);
		static bool IsLower(_L_Char c);
		static bool IsSymbol(_L_Char c);
		static bool IsUpper(_L_Char c);
		static _L_Char ToLowerInvariant(_L_Char c);
		static _L_Char ToUpperInvariant(_L_Char c);
	};
}
#endif