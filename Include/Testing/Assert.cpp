// File: Assert.cpp
// Author: Rendong Liang (Liong)
#include "Assert.hpp"
#include "UnitTest.hpp"

_L_NS_BEG_TESTING

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

_L_NS_END_TESTING
