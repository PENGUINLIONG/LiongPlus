// File: DateTime.cpp
// Author: Rendong Liang (Liong)
#include <chrono>
#include "LiongPlus/DateTime.hpp"
#include "LiongPlus/Buffer.hpp"

_L_NS_BEG

namespace DateTime
{
	using namespace std;

	time_t Now()
	{
		using namespace std::chrono;
		return system_clock::to_time_t(system_clock::now());
	}

	enum
	{
		__MinBufferSize = 32,
	};

	string GetDateTime(const string& format, const tm* timeFactors)
	{
		static auto buffer = Buffer(__MinBufferSize);

		auto len = strftime((char*)(buffer.Field()), buffer.Length(), format.c_str(), timeFactors);
		if (len == 0)
			buffer = Buffer(buffer.Length() * 2);
		else
			return string((char*)buffer.Field());
		return (const char*)buffer.Field();
	}

	string GetRfc1123(time_t t)
	{
		return GetDateTime("%a, %d %b %Y %H:%M:%S GMT", gmtime(&t));
	}

	string GetRfc850(time_t t)
	{
		return GetDateTime("%A, %d-%b-%y %H:%M:%S GMT", gmtime(&t));
	}

	string GetAsctimeGmt(time_t t)
	{
		return asctime(gmtime(&t));
	}

	string GetCustomized(const string& format, time_t t)
	{
		return GetDateTime(format, gmtime(&t));
	}
}

_L_NS_END