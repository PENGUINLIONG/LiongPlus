#pragma once
// File: DateTime.cpp
// Author: Rendong Liang (Liong)

#ifndef _L_DateTime_Impl
#define _L_DateTime_Impl
#include "DateTime.hpp"

namespace LiongPlus
{
	const int DateTime::_DaysToMonth_365[12] = { 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
	const int DateTime::_DaysToMonth_366[12] = { 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
}
#endif