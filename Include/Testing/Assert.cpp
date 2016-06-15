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
				UnitTest::Results.Last().State = TestState::Passed;
				*UnitTest::Results.Last().Log << "[Passed]";
			}
			else
			{
				UnitTest::Results.Last().State = TestState::Failed;
				*UnitTest::Results.Last().Log << "[Assertion failed]";
			}
		}
	}
}