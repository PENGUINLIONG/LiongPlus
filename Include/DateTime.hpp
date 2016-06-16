// File: DateTime.hpp
// Author: Rendong Liang (Liong)

#pragma once
#include "Fundamental.hpp"
#include "Buffer.hpp"

namespace LiongPlus
{
	using namespace std;
	
	class DateTime
	{
	private:
		static const size_t BUFFER_LENGTH = 128;
		static mutex _M;

		static string GetDateTime(const char* format, const tm* timeFactors);
	public:
		static string GetRfc1123(time_t t);
		static string GetRfc850(time_t t);
		static string GetAsctimeGmt(time_t t);
		static string GetCustomized(const char* format, time_t t);
	};
}
