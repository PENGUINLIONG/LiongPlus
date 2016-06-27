// File: HttpHeader.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "../Fundamental.hpp"

namespace LiongPlus
{
	namespace Net
	{
		class HttpUtils
		{
		public:
			static bool IsLineSeparator(const char* pos);
			static bool IsCrLf(const char* pos);
			static bool IsLWS(const char* pos);
			static char* SeekForEOL(const char* beg, const char* end);
			static char* SeekForNonLWS(const char* beg, const char* end);
			static void CaptureText(char*& pos, const char* eol, std::string& result);
			static void CaptureNumber(char*& pos, const char* eol, long& result);
			static bool CaptureHttpVersion(char*& pos, const char* eol, long& major, long& minor);
			static size_t GetOffset(const char* beg, const char* eol, const char* end);
		};
	}
}