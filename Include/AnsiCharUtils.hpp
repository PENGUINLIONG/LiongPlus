// File: AnsiCharUtils.hpp
// Author: Rendong Liang (Liong)
#include "Fundamental.hpp"

namespace LiongPlus
{
	class AnsiCharUtils
	{
	public:
		static bool IsLowerCase(const char c)
		{
			return c >= 'a' && c <= 'z';
		}

		static bool IsUpperCase(const char c)
		{
			return c >= 'A' && c <= 'Z';
		}

		static bool IsAlphabet(const char c)
		{
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		}

		static bool IsDigit(const char c)
		{
			return c >= '0' && c <= '9';
		}

		static bool IsAlphaDigit(const char c)
		{
			return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
		}
	};
}