// File: UnitTest.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include "_"

#ifndef __FUNCTIONW__
#define __FUNCTIONW__ __FUNCTION__
#endif

_L_NS_BEG_TESTING
enum class TestState
{
	Waiting,
	Passed,
	Failed,
	Skipped
};

class TestObject
{
public:
	TestObject();
	TestObject(const TestObject&) = delete;
	TestObject(TestObject&&) = delete;
	virtual ~TestObject();

	TestObject& operator=(TestObject&&) = delete;

	virtual void Prepare();
	virtual void Test() = 0;
	virtual void CleanUp();
};

struct TestResult
{
	std::shared_ptr<std::stringstream> Log;
	std::string Name;
	TestState State;

	TestResult();
	TestResult(std::string name);
	TestResult(const TestResult& instance);
	TestResult(TestResult&& instance);

	TestResult& operator=(const TestResult& instance);
	TestResult& operator=(TestResult&& instance);

	bool operator==(TestResult& value);
};

class UnitTest
{
public:
	static class ResultsObject
	{
	public:
		TestResult& operator[](long index)
		{
			return _Results[index];
		}

		void Add(TestResult& result)
		{
			_Results.push_back(result);
		}

		TestResult& Last()
		{
			return _Results.back();
		}
	} Results;

	static void Test(TestObject& obj);

	static void RunUnit(std::function<void(void)> unit);

	static std::string Summary();
	static std::vector<int> ListResultId(TestState state);
private:
	static std::mutex _Mutex;
	static std::vector<TestResult> _Results;
};

_L_NS_END_TESTING

#define _L_Test_Class(name) class name : public LiongPlus::Testing::TestObject
#define _L_Test_Prepare virtual void Prepare() override final
#define _L_Test_TestList virtual void Test() override final
#define _L_Test_CleanUp virtual void CleanUp() override final
#define _L_Test_Unit_Static(funcName) LiongPlus::Testing::UnitTest::Results.Add(TestResult("##funcName##")), LiongPlus::Testing::UnitTest::RunUnit(funcName);
#define _L_Test_Unit(funcName) LiongPlus::Testing::UnitTest::Results.Add(TestResult("##funcName##")), LiongPlus::Testing::UnitTest::RunUnit(std::bind(funcName, this));
#define _L_Test_Unit_Lambda(name, func) LiongPlus::Testing::UnitTest::Results.Add(TestResult(name)), LiongPlus::Testing::UnitTest::RunUnit(func);