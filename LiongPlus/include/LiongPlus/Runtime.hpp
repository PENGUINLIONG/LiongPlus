// File: Runtime.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <cstdio>
#include <string>
#include <thread>
#include "_"
#include "DateTime.hpp"

_L_NS_BEG

namespace ErrorLevel
{
	enum class Info {};
	enum class Success {};
	enum class Warning {};
	enum class Error {};
}
namespace Logger
{
	template<typename ... TArgs>
	void LogInfo(const std::string& str, TArgs&& ... args)
	{
		using namespace std;
		std::printf("%s %s ",
			DateTime::GetRfc1123(DateTime::Now()).c_str(),
			"[INFO]");
		std::printf(str.c_str(), std::forward<TArgs>(args) ...);
		std::printf("\r\n");
	}
	template<typename ... TArgs>
	void LogSuccess(const std::string& str, TArgs&& ... args)
	{
		using namespace std;
		std::printf("%s %s ",
			DateTime::GetRfc1123(DateTime::Now()).c_str(),
			"[SUCCESS]");
		std::printf(str.c_str(), std::forward<TArgs>(args) ...);
		std::printf("\r\n");
	}
	template<typename ... TArgs>
	void LogWarning(const std::string& str, TArgs&& ... args)
	{
		using namespace std;
		std::printf("%s %s ",
			DateTime::GetRfc1123(DateTime::Now()).c_str(),
			"[WARNING]");
		std::printf(str.c_str(), std::forward<TArgs>(args) ...);
		std::printf("\r\n");
	}
	template<typename ... TArgs>
	void LogError(const std::string& str, TArgs&& ... args)
	{
		using namespace std;
		std::printf("%s %s ",
			DateTime::GetRfc1123(DateTime::Now()).c_str(),
			"[ERROR]");
		std::printf(str.c_str(), std::forward<TArgs>(args) ...);
		std::printf("\r\n");
	}

	// Default error level.
	template<typename ... TArgs>
	void Log(const std::string& str, TArgs&& ... args)
	{
		LogInfo(str.c_str(), std::forward<TArgs>(args) ...);
	}

}
_L_NS_END
