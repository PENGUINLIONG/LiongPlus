// File: UnitTest.hpp
// Author: Rendong Liang (Liong)

#ifndef _L_UnitTest
#define _L_UnitTest
#include "../Fundamental.hpp"
#include "../Delegate.hpp"
#include "../String.hpp"
#include "../Text/StringBuilder.hpp"
#include "../Collections/List.hpp"
#include <mutex>

#ifndef __FUNCTIONW__
#define __FUNCTIONW__ __FUNCTION__
#endif

using namespace LiongPlus::Collections;
using namespace LiongPlus::Text;

namespace LiongPlus
{
	namespace Testing
	{
		enum class TestState
		{
			Waiting,
			Passed,
			Failed,
			Skipped
		};

		class TestObject
			: public Object
		{
		public:
			TestObject();
			TestObject(const TestObject&) = delete;
			TestObject(TestObject&&) = delete;
			virtual ~TestObject();

			TestObject& operator=(const TestObject&) = delete;
			TestObject& operator=(TestObject&&) = delete;

			virtual void Prepare();
			virtual void Test() = 0;
			virtual void CleanUp();
		};

		struct TestResult
		{
			StringBuilder Log;
			String Name;
			TestState State;

			TestResult();
			TestResult(String name);
			TestResult(const TestResult& instance);
			TestResult(TestResult&& instance);

			TestResult& operator=(const TestResult& instance);
			TestResult& operator=(TestResult&& instance);

			bool operator==(TestResult& value);
		};

		class UnitTest
			: public Object
		{
		public:
			static std::mutex _Mutex;
			static List<TestResult> _Results;

			static void Test(TestObject& obj);

			static void RunUnit(Delegate<void(void)> unit);

			static String Summary();
			static List<String> ListResult(TestState state);
		};
	}
}

#define _L_Test_Class(name) class name : public LiongPlus::Testing::TestObject
#define _L_Test_Prepare virtual void Prepare() override final
#define _L_Test_TestList virtual void Test() override final
#define _L_Test_CleanUp virtual void CleanUp() override final
#define _L_Test_Unit(name, func) LiongPlus::Testing::UnitTest::_Results.Add(TestResult(_LT(name))); LiongPlus::Testing::UnitTest::RunUnit(func)
#endif