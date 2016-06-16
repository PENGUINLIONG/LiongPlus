// File: DateTime.cpp
// Author: Rendong Liang (Liong)
#include "DateTime.hpp"

namespace LiongPlus
{
	using namespace std;

	mutex DateTime::_M;

	string DateTime::GetDateTime(const char* format, const tm* timeFactors)
	{
		static char buffer[BUFFER_LENGTH];

		auto len = strftime(buffer, BUFFER_LENGTH, format, timeFactors);
		if (len == 0)
			return "BUFFER TOO SHORT";
		else
			return buffer;
	}

	string DateTime::GetRfc1123(time_t t)
	{
		_M.lock();
		auto rv = GetDateTime("%a, %d %b %Y %H:%M:%S GMT", gmtime(&t));
		_M.unlock();
		return rv;
	}

	string DateTime::GetRfc850(time_t t)
	{
		_M.lock();
		auto rv = GetDateTime("%A, %d-%b-%y %H:%M:%S GMT", gmtime(&t));
		_M.unlock();
		return rv;
	}

	string DateTime::GetAsctimeGmt(time_t t)
	{
		_M.lock();
		auto rv = asctime(gmtime(&t));
		_M.unlock();
		return rv;
	}

	string DateTime::GetCustomized(const char* format, time_t t)
	{
		_M.lock();
		auto rv = GetDateTime(format, gmtime(&t));
		_M.unlock();
		return rv;
	}
}