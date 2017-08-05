// File: DateTime.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include <ctime>
#include <string>
#include "_"

_L_NS_BEG

namespace DateTime
{
	time_t Now();

	std::string GetRfc1123(time_t t);
	std::string GetRfc850(time_t t);
	std::string GetAsctimeGmt(time_t t);
	std::string GetCustomized(const std::string& format, time_t t);
}

_L_NS_END
