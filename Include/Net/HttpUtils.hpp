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
			static bool IsLineSeparator(const char* pos)
			{
				return *pos == '\r' || *pos == '\n';
			}

			static bool IsCrLf(const char* pos)
			{
				return (pos[0] == '\r' && pos[1] == '\n');
			}

			static bool IsLWS(const char* pos)
			{
				return *pos == ' ' || *pos == '\t';
			}

			static char* SeekForEOL(const char* beg, const char* end)
			{
				char* eol = const_cast<char*>(beg);
				while (!HttpUtils::IsLineSeparator(eol) && eol < end)
					++eol;
				return eol;
			}

			static char* SeekForNonLWS(const char* beg, const char* end)
			{
				char* nlws = const_cast<char*>(beg);
				while (HttpUtils::IsLWS(nlws) && nlws < end)
					++nlws;
				return nlws;
			}

			static void CaptureText(char*& pos, const char* eol, std::string& result)
			{
				const char* beg = pos;
				while (pos < eol)
				{
					if (*pos == ' ')
						break;
					++pos;
				}
				result = std::string(beg, pos);
			}

			static void CaptureNumber(char*& pos, const char* eol, long& result)
			{
				while (*pos >= '0' && *pos <= '9' && pos < eol)
					result = result * 10 + (*(pos++) - '0');
			}

			static bool CaptureHttpVersion(char*& pos, const char* eol, long& major, long& minor)
			{
				if ((pos + 8 >= eol) ||
					(pos[0] != 'H' || pos[1] != 'T' || pos[2] != 'T' || pos[3] != 'P' || pos[4] != '/'))
					return false;
				pos += 5;
				CaptureNumber(pos, eol, major);
				if (*(pos++) != '.') return false;
				CaptureNumber(pos, eol, minor);

				return true;
			}

			static size_t GetOffset(const char* beg, const char* eol, const char* end)
			{
				if (eol < end)
				{
					if (eol + 1 < end && HttpUtils::IsCrLf(eol))
						return (eol - beg) + 2;
					else
						return (eol - beg) + 1;
				}
				else
					return (eol - beg);
			}
		};
	}
}