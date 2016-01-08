// File: UnitTest.cpp
// Author: Rendong Liang (Liong)
#include "UnitTest.hpp"

namespace LiongPlus
{
	namespace Testing
	{
		std::mutex UnitTest::_Mutex;
		List<TestResult> UnitTest::_Results;

		TestObject::TestObject()
		{
		}
		TestObject::~TestObject()
		{
		}

		void TestObject::Prepare()
		{
		}
		void TestObject::CleanUp()
		{
		}



		TestResult::TestResult()
			: Log(String())
			, Name(String())
			, State(TestState::Waiting)
		{
		}
		TestResult::TestResult(String name)
			: Log(String())
			, Name(name)
			, State(TestState::Waiting)
		{
		}
		TestResult::TestResult(const TestResult& instance)
			: Log(instance.Log)
			, Name(instance.Name)
			, State(instance.State)
		{
		}
		TestResult::TestResult(TestResult&& instance)
			: Log(String())
			, Name(String())
			, State(TestState::Waiting)
		{
			Swap(Log, instance.Log);
			Swap(Name, instance.Name);
			Swap(State, instance.State);
		}

		TestResult& TestResult::operator=(const TestResult& instance)
		{
			Log = instance.Log;
			Name = instance.Name;
			State = instance.State;
			return *this;
		}
		TestResult& TestResult::operator=(TestResult&& instance)
		{
			Swap(Log, instance.Log);
			Swap(Name, instance.Name);
			Swap(State, instance.State);
			return *this;
		}

		bool TestResult::operator==(TestResult& value)
		{
			return Name == value.Name;
		}



		void UnitTest::Test(TestObject& obj)
		{
			obj.Test();
		}

		void UnitTest::RunUnit(Delegate<void(void)> unit)
		{
			UnitTest::_Mutex.lock();
			try
			{
				unit();
			}
			catch (...)
			{
				_Results.Last().Log.AppendLine(String(_LT("[Exception occured, please debug this test]")));
				_Results.Last().State = TestState::Failed;
				return;
			}
			if (_Results.Last().State == TestState::Waiting)
				_Results.Last().State = TestState::Passed;
			UnitTest::_Mutex.unlock();
		}

		String UnitTest::Summary()
		{
			int passed = 0;
			for (auto& result : _Results)
			{
				if (result.State == TestState::Passed)
					++passed;
			}
			return String::FromValue(_Results.Count()) + _LT(" tests have been run and ") + String::FromValue(passed) + _LT(" of these passed.");
		}
		List<String> UnitTest::ListResult(TestState state)
		{
			List<String> list;
			for (auto& result : _Results)
			{
				if (result.State == state)
					list.Add(result.Name);
			}
			return list;
		}
	}
}