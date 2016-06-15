// File: Char.cpp
// Author: Rendong Liang (Liong)
#include "Char.hpp"

namespace LiongPlus
{
	const _L_Char Char::NextLine = _LT('\n');
	const _L_Char Char::EndOfString = _LT('\0');

	bool Char::IsControl(_L_Char c)
	{
		return (c >= 0 && c <= 0x1F) || c == 0x7F;
	}
	
	bool Char::IsDigit(_L_Char c)
	{
		return c >= _LT('0') && c <= _LT('9');
	}
	
	bool Char::IsLetter(_L_Char c)
	{
		return IsLower(c) || IsUpper(c);
	}
	
	bool Char::IsLetterOrDigit(_L_Char c)
	{
		return IsLetter(c) || IsDigit(c);
	}
	
	bool Char::IsLower(_L_Char c)
	{
		return c >= _LT('a') && c <= _LT('z');
	}
	
	bool Char::IsSymbol(_L_Char c)
	{
		return (c >= 0x21 && c <= 0x2F) || (c >= 0x3A && c <= 0x40) || (c >= 0x5B && c <= 0x60) || (c >= 0x7B && c <= 0x7E);
	}
	
	bool Char::IsUpper(_L_Char c)
	{
		return c >= _LT('A') && c <= _LT('Z');
	}
	
	_L_Char Char::ToLowerInvariant(_L_Char c)
	{
		if (IsUpper(c))
			return c + 0x20;
		else return c;
	}
	
	_L_Char Char::ToUpperInvariant(_L_Char c)
	{
		if (IsLower(c))
			return c - 0x20;
		else return c;
	}
}
