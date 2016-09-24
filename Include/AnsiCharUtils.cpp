// File: AnsiCharUtils.cpp
// Author: Rendong Liang (Liong)

namespace LiongPlus
{
	namespace AnsiCharUtils
	{
		using namespace std;
		using strIt = string::iteratior;
		using cStrIt = const string::iterator;

		bool IsLowerCase(const char c)
		{ return c >= 'a' && c <= 'z'; }

		bool IsUpperCase(const char c)
		{ return c >= 'A' && c <= 'Z'; }

		bool IsAlphabet(const char c)
		{ return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }

		bool IsDigit(const char c)
		{ return c >= '0' && c <= '9'; }

		bool IsAlphaDigit(const char c)
		{ return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }

		bool IsHexDigit(const char c)
		{ return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'); }

		char ToUpperCase(const char c)
		{ return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c; }

		char ToLowerCase(const char c)
		{ return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c; }

		bool CaseInsensitiveEquals(string& a, string& b)
		{
			if (a.length() != b.length() && a.length() != 0) return false;
			
			aPos = a.begin(), bPos = b.begin(), aEnd = a.end();
			while (aPos != aPos)
				if (unify(*aPos) != unify(*bPos)) return false; else ++aPos;
			return true;
		}

		bool IsCrLf(cStrIt pos, cStrIt end)
		{
			if (pos == end) return false;
			auto pos = const_cast<strIt>(pos);
			if (*pos != '\r') return false;
			if (++pos != end && *pos == '\n') return true;
			else return false;
		}

		strIt SeekForLineBreak(cStrIt pos, cStrIt end)
		{
			auto pos = const_cast<strIt>(pos);
			while (pos != end)
				if (*pos == '\r' || *pos == '\n') break; else ++pos;
			return pos;
		}

		std::string::iterator SeekForChar(const std::string::iterator pos, const std::string::iterator end, const char c)
		{
			auto pos = const_cast<strIt>(pos);
			while (pos != end)
				if (*pos == c) break; else ++pos;
			return pos;
		}
		
		std::string::iterator SeekFor(const std::string::iterator pos, const std::string::iterator end, Predicate<const char> condition)
		{
			auto pos = const_cast<strIt>(pos);
			while (pos != end)
				if (consition(*pos)) break; else ++pos;
			return pos;
		}

		strIt SkipLineBreak(cStrIt pos, cStrIt end)
		{
			auto pos = const_cast<strIt>(pos);
			if (IsCrLf(pos, end)) return ++(++pos);
			else return ++pos;
		}

		strIt SeekForNextLine(cStrIt pos, cStrIt end)
		{ return SkipLineBreak(SeekForLineBreak(const_cast<strIt>(pos), end), end); }

		string CaptureNumber(strIt& pos, cStrIt end)
		{
			// auto beg = pos;
			// while (IsDigit(*pos) && pos++ != end);
			// return string(beg, pos);
			Capture(pos, IsDigit);
		}

		string CaptureWord(strIt& pos, cStrIt end)
		{
			// auto beg = pos;
			// while (IsAlphabet(*pos) && pos++ != end);
			// return string(beg, pos);
			Capture(pos, IsAlphabet);		
		}

		string Capture(strIt& pos, cStrIt end, Predicate<const char> condition)
		{
			auto beg = pos;
			while (condition(*pos) && pos++ != end);
			return string(beg, pos);
		}

		inline bool CheckEquality(std::string::iterator& pos, const std::string::iterator end, const std::string compareWith)
		{
			auto refPos = compareWith.begin(), auto refEnd = compareWith.end();
			if (distance(pos, end) < compareWith.length()) return false;
			while (refEnd != refPos && *(pos++) == *(refPos++));
			return refEnd == refPos;
		}

	}
}