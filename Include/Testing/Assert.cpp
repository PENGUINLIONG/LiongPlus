// File: Assert.cpp
// Author: Rendong Liang (Liong)
#include "Assert.hpp"

namespace LiongPlus
{
	namespace Testing
	{
		void Assert::Discriminate(bool isTrue)
		{
			if (isTrue)
			{
				UnitTest::_Results.Last().State = TestState::Passed;
				UnitTest::_Results.Last().Log.AppendLine(String(_LT("[Passed]")));
			}
			else
			{
				UnitTest::_Results.Last().State = TestState::Failed;
				UnitTest::_Results.Last().Log.AppendLine(String(_LT("[Assertion failed]")));
			}
		}
	}
}