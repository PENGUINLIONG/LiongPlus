// File: DateTime.cpp
// Author: Rendong Liang (Liong)
#include "DateTime.hpp"
#include "Buffer.hpp"

_L_NS_BEG

using namespace std;

string DateTime::GetDateTime(const string& format, const tm* timeFactors)
{
	static auto buffer = Buffer(64);

	auto len = strftime((char*)(buffer.Field()), buffer.Length(), format.c_str(), timeFactors);
	if (len == 0)
		buffer = Buffer(buffer.Length() * 2);
	else
		return string((char*)buffer.Field());
}

string DateTime::GetRfc1123(time_t t)
{
	return GetDateTime("%a, %d %b %Y %H:%M:%S GMT", gmtime(&t));
}

string DateTime::GetRfc850(time_t t)
{
	return GetDateTime("%A, %d-%b-%y %H:%M:%S GMT", gmtime(&t));
}

string DateTime::GetAsctimeGmt(time_t t)
{
	return asctime(gmtime(&t));
}

string DateTime::GetCustomized(const string& format, time_t t)
{
	return GetDateTime(format, gmtime(&t));
}

_L_NS_END