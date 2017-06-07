// File: Assume.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include "_"
#include "UnitTest.hpp"

_L_NS_BEG_TESTING

class Assume
{
public:
	template<typename T>
	static void Equals(T actual, T expectance)
	{
		if (actual != expectance)
		{
			UnitTest::_Results.Last().State = TestState::Skipped;
			UnitTest::_Results.Last().Log.AppendLine(std::string("[Invalid Input(s), skip]"));
		}
	}

	template<typename T, typename ... TArgs>
	static void NoException(T func, TArgs ... args)
	{
		try
		{
			func(args ...);
		}
		catch (...)
		{
			UnitTest::_Results.Last().State = TestState::Skipped;
			UnitTest::_Results.Last().Log.AppendLine(std::string("[Invalid Input(s), skip]"));
		}
	}
};

_L_NS_END_TESTING