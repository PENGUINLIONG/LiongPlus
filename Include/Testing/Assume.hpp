// File: Assume.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_Assume
#define _L_Assume
#include "../Fundamental.hpp"
#include "UnitTest.hpp"

namespace LiongPlus
{
	namespace Testing
	{
		class Assume
		{
		public:
			template<typename T>
			static void Equals(T actual, T expectance)
			{
				if (actual != expectance)
				{
					UnitTest::_Results.Last().State = TestState::Skipped;
					UnitTest::_Results.Last().Log.AppendLine(String(_LT("[Invalid Input(s), skip]")));
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
					UnitTest::_Results.Last().Log.AppendLine(String(_LT("[Invalid Input(s), skip]")));
				}
			}
		};
	}
}
#endif