// File: HttpUtils.hpp
// Author: Rendong Liang (Liong)
#include "HttpUtils.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;

		bool HttpUtils::IsLineSeparator(const char* pos)
		{
			return *pos == '\r' || *pos == '\n';
		}

		bool HttpUtils::IsCrLf(const char* pos)
		{
			return (pos[0] == '\r' && pos[1] == '\n');
		}

		bool HttpUtils::IsLWS(const char* pos)
		{
			return *pos == ' ' || *pos == '\t';
		}

		char* HttpUtils::SeekForEOL(const char* beg, const char* end)
		{
			char* eol = const_cast<char*>(beg);
			while (!HttpUtils::IsLineSeparator(eol) && eol < end)
				++eol;
			return eol;
		}

		char* HttpUtils::SeekForNonLWS(const char* beg, const char* end)
		{
			char* nlws = const_cast<char*>(beg);
			while (HttpUtils::IsLWS(nlws) && nlws < end)
				++nlws;
			return nlws;
		}

		void HttpUtils::CaptureText(char*& pos, const char* eol, std::string& result)
		{
			const char* beg = pos;
			while (pos < eol)
			{
				if (*pos == ' ')
					break;
				++pos;
			}
			result = std::string(beg, pos - beg);
		}

		void HttpUtils::CaptureNumber(char*& pos, const char* eol, long& result)
		{
			while (*pos >= '0' && *pos <= '9' && pos < eol)
				result = result * 10 + (*(pos++) - '0');
		}

		bool HttpUtils::CaptureHttpVersion(char*& pos, const char* eol, long& major, long& minor)
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

		size_t HttpUtils::GetOffset(const char* beg, const char* eol, const char* end)
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
	}
}