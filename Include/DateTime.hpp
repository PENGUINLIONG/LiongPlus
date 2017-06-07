// File: DateTime.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include <ctime>
#include <mutex>
#include <string>
#include "_"

_L_NS_BEG

class DateTime
{
private:
	static const size_t _MinBufferSize = 64;

	static std::string GetDateTime(const std::string& format, const std::tm* timeFactors);

public:
	static std::string GetRfc1123(time_t t);
	static std::string GetRfc850(time_t t);
	static std::string GetAsctimeGmt(time_t t);
	static std::string GetCustomized(const std::string& format, time_t t);
};

_L_NS_END
