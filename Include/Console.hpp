// File: Console.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Console
#define _L_Console
#include "Fundamental.hpp"
#include "Char.hpp"
#include "String.hpp"

namespace LiongPlus
{
	enum class ConsoleColor
	{
		Black = 0,
		DarkBlue = 1,
		DarkGreen = 2,
		DarkCyan = 3,
		DarkRed = 4,
		DarkMagenta = 5,
		DarkYellow = 6,
		Gray = 7,
		DarkGray = 8,
		Blue = 9,
		Green = 10,
		Cyan = 11,
		Red = 12,
		Magenta = 13,
		Yellow = 14,
		White = 15
	};

	class Console
	{
	public:
		static void Beep();

		static void Clear();

		static void Read();

		static void Write(String& value);
		static void Write(bool value);
		static void Write(int value);
		static void Write(unsigned int value);
		static void Write(const _L_Char* c_str);

		static void WriteLine();
		static void WriteLine(String& value);
		static void WriteLine(bool value);
		static void WriteLine(int value);
		static void WriteLine(unsigned int value);
		static void WriteLine(const _L_Char* c_str);
	};
}
#endif