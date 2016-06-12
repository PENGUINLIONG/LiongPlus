// File: UnitTest.cpp
// Author: Rendong Liang (Liong)
#include "UnitTest.hpp"

namespace LiongPlus
{
	namespace Testing
	{
		using std::swap;

		std::mutex UnitTest::_Mutex;
		std::vector<TestResult> UnitTest::_Results;
		UnitTest::ResultsObject UnitTest::Results;

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
			: Log()
			, Name(std::string())
			, State(TestState::Waiting)
		{
		}
		TestResult::TestResult(std::string name)
			: Log()
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
			: TestResult()
		{
			swap(Log, instance.Log);
			swap(Name,  instance.Name);
			swap(State, instance.State);
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
			swap(Log, instance.Log);
			swap(Name, instance.Name);
			swap(State, instance.State);
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

		void UnitTest::RunUnit(std::function<void(void)> unit)
		{
			UnitTest::_Mutex.lock();
			try
			{
				unit();
			}
			catch (...)
			{
				*_Results.back().Log << "[Exception occured, please debug this test]";
				_Results.back().State = TestState::Failed;
				return;
			}
			if (_Results.back().State == TestState::Waiting)
				_Results.back().State = TestState::Passed;
			UnitTest::_Mutex.unlock();
		}

		std::string UnitTest::Summary()
		{
			int passed = 0;
			for (auto& result : _Results)
			{
				if (result.State == TestState::Passed)
					++passed;
			}
			return std::to_string(_Results.size()) + " tests have been run and " + std::to_string(passed) + " of these passed.";
		}
		std::vector<int> UnitTest::ListResultId(TestState state)
		{
			_Mutex.lock();
			std::vector<int> list;
			for (int i = 0; i < _Results.size(); ++i)
			{
				if (_Results[i].State == state)
					list.push_back(i);
			}
			_Mutex.unlock();
			return list;
		}
	}
}