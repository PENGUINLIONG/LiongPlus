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
	private:
		// Unicode category values from Unicode U+0000 ~ U+00FF. Store them in byte[] array to save space.
		const static byte Char::_CategoryForLatin1[];
	public:
		const static _L_Char NextLine = _LT('\n');
		const static _L_Char EndOfString = _LT('\0');
		/// <summary>
		/// NOTICE: intStudio::_L_Char is a static class.
		/// </summary>
		Char() = delete;

		static double GetNumericValue(_L_Char c);
		// The following methods are designed for
		static bool IsControl(_L_Char c);
		static bool IsDigit(_L_Char c);
		static bool IsHighSurrogate(_L_Char c);
		static bool IsLetter(_L_Char c);
		static bool IsLetterOrDigit(_L_Char c);
		static bool IsLower(_L_Char c);
		static bool IsLowSurrogate(_L_Char c);
		static bool IsNumber(_L_Char c);
		static bool IsPunctuation(_L_Char c);
		static bool IsSeparator(_L_Char c);
		static bool IsSurrogate(_L_Char c);
		static bool IsSurrogatePair(_L_Char c);
		static bool IsSymbol(_L_Char c);
		static bool IsUpper(_L_Char c);
		static bool IsWhiteSpace(_L_Char c);
		static _L_Char ToLowerInvariant(_L_Char c);
		static _L_Char ToUpperInvariant(_L_Char c);

	};
}
#endif