// File: Console.cpp
// Author: Rendong Liang (Liong)
#include "Console.hpp"

namespace LiongPlus
{
	void Console::Beep()
	{
#ifdef _L_WINDOWS
		::Beep(800, 200);
#elif _L_LINUX
		_L_putchar('\a');
#endif
	}

	void Console::Clear()
	{
#ifdef _L_WINDOWS
		system("cls");
#elif _L_LINUX
		system("clear");
#endif
		
	}

	void Console::Read()
	{

	}

	void Console::Write(String & value)
	{
		_L_fputs(value.GetNativePointer(), stdout);
	}
	void Console::Write(bool value)
	{
		Write(String::FromValue(value));
	}
	void Console::Write(long value)
	{
		Write(String::FromValue(value));
	}
	void Console::Write(unsigned long value)
	{
		Write(String::FromValue(value));
	}
	void Console::Write(const _L_Char* c_str)
	{
		_L_fputs(c_str, stdout);
	}

	void Console::WriteLine()
	{
		_L_putchar(Char::NextLine);
	}
	void Console::WriteLine(String & value)
	{
		_L_puts(value.GetNativePointer());
	}
	void Console::WriteLine(bool value)
	{
		WriteLine(String::FromValue(value));
	}
	void Console::WriteLine(long value)
	{
		WriteLine(String::FromValue(value));
	}
	void Console::WriteLine(unsigned long value)
	{
		WriteLine(String::FromValue(value));
	}
	void Console::WriteLine(const _L_Char* c_str)
	{
		_L_puts(c_str);
	}
}
