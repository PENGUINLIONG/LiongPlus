// File: HttpLine.cpp
// Author: Rendong Liang (Liong)
#include <sstream>
#include "LiongPlus/AnsiCharUtils.hpp"
#include "LiongPlus/Net/Http/HttpLine.hpp"

_L_NS_BEG_NET_HTTP

using namespace std;
using std::swap;

long ExceptionalToLong(string str)
{
	return stol(str == "" ? 0 : str);
}

bool CaptureHttpVersion(const char* pos, const char* end, long& majorVersion, long& minorVersion)
{
	if (std::distance(pos, end) < sizeof("HTTP/1.0") ||
		!AnsiCharUtils::SectionalEquals(pos, end, "HTTP/", "HTTP/" + sizeof("HTTP/") - 1))
		return false;
	pos += sizeof("HTTP/");
	majorVersion = ExceptionalToLong(AnsiCharUtils::CaptureDigits<string>(pos, end));
	if (*(pos++) != '.') return false;
	minorVersion = ExceptionalToLong(AnsiCharUtils::CaptureDigits<string>(pos, end));
}

//
// HttpLine
//

HttpLine::HttpLine(long major, long minor)
	: MajorVersion(major)
	, MinorVersion(minor)
{
}

//
// HttpMethods
//

namespace HttpMethods
{
	const char
		*Get = "GET",
		*Head = "HEAD",
		*Post = "POST",
		*Put = "PUT",
		*Delete = "DELETE",
		*Trace = "TRACE",
		*Options = "OPTIONS",
		*Connect = "CONNECT",
		*Patch = "PATCH";
}

//
// HttpResponseLine
//

HttpRequestLine::HttpRequestLine(HttpRequestLine&& instance)
	: HttpRequestLine()
{
	swap(MajorVersion, instance.MajorVersion);
	swap(MinorVersion, instance.MinorVersion);
	swap(Method, instance.Method);
	swap(Path, instance.Path);
}
HttpRequestLine::HttpRequestLine(long major, long minor, string method, string path)
	: HttpLine(major, minor)
	, Method(method)
	, Path(path)
{
}

HttpRequestLine& HttpRequestLine::operator=(const HttpRequestLine& instance)
{
	MajorVersion = instance.MajorVersion;
	MinorVersion = instance.MinorVersion;
	Method = instance.Method;
	Path = instance.Path;
	return *this;
}
HttpRequestLine& HttpRequestLine::operator=(HttpRequestLine&& instance)
{
	swap(MajorVersion, instance.MajorVersion);
	swap(MinorVersion, instance.MinorVersion);
	swap(Method, instance.Method);
	swap(Path, instance.Path);
	return *this;
}

string HttpRequestLine::ToString() const
{
	stringstream ss;
	ss << Method << ' ' << Path << ' ' << "HTTP/" << MajorVersion << '.' << MinorVersion << '\n';
	return ss.str();
}

// Notice that the offset will be set to the beginning of the HTTP header part.
bool HttpRequestLine::FromBuffer(Buffer& buffer, size_t& offset)
{
	HttpRequestLine line;
	auto end = buffer.Field() + buffer.Length(); // Should not be read.
	auto beg = buffer.Field() + offset;
	auto pos = beg;
	auto eol = AnsiCharUtils::SeekForLineBreak(beg, end);

	line.Method = AnsiCharUtils::CaptureNonWhiteSpace<string>(pos, eol);
	if (*(pos++) != ' ') return false;
	line.Path = AnsiCharUtils::CaptureNonWhiteSpace<string>(pos, eol);
	if (*(pos++) != ' ') return false;
	if (!CaptureHttpVersion((char*)pos, (char*)eol, line.MajorVersion, line.MinorVersion))
		return false;
	// Here should be the end of line.
	if (pos != eol) return false;
	// Set offset.
	offset += distance(beg, AnsiCharUtils::SkipLineBreak(eol, end));

	*this = std::move(line);
	return true;
}

//
// HttpResponseLine
//

HttpStatusLine::HttpStatusLine(HttpStatusLine&& instance)
	: HttpStatusLine()
{
	swap(MajorVersion, instance.MajorVersion);
	swap(MinorVersion, instance.MinorVersion);
	swap(StatusCode, instance.StatusCode);
	swap(Reason, instance.Reason);
}
HttpStatusLine::HttpStatusLine(long major, long minor, long statusCode, string reason)
	: HttpLine(major, minor)
	, StatusCode(statusCode)
	, Reason(reason)
{
}

HttpStatusLine& HttpStatusLine::operator=(const HttpStatusLine& instance)
{
	MajorVersion = instance.MajorVersion;
	MinorVersion = instance.MinorVersion;
	StatusCode = instance.StatusCode;
	Reason = instance.Reason;
	return *this;
}
HttpStatusLine& HttpStatusLine::operator=(HttpStatusLine&& instance)
{
	swap(MajorVersion, instance.MajorVersion);
	swap(MinorVersion, instance.MinorVersion);
	swap(StatusCode, instance.StatusCode);
	swap(Reason, instance.Reason);
	return *this;
}

string HttpStatusLine::ToString() const
{
	stringstream ss;
	ss << "HTTP/" << MajorVersion << '.' << MinorVersion << ' ' << StatusCode << ' ' << Reason << '\n';
	return ss.str();
}

// Notice that the offset will be set to the beginning of the HTTP header part.
bool HttpStatusLine::FromBuffer(Buffer buffer, size_t& offset)
{
	HttpStatusLine line;
	auto end = buffer.Field() + buffer.Length(); // Should not be read.
	auto beg = buffer.Field() + offset;
	auto pos = beg;
	auto eol = AnsiCharUtils::SeekForLineBreak(beg, end);

	if (!CaptureHttpVersion((char*)pos, (char*)eol, line.MajorVersion, line.MinorVersion))
		return false;
	if (*(pos++) != ' ') return false;
	line.StatusCode = ExceptionalToLong(AnsiCharUtils::CaptureDigits<string>(pos, eol));
	if (*(pos++) != ' ') return false;
	line.Reason = string(pos, eol);
	// Here should be the end of line but we will do nothing.
	// (Because $pos must equal to $eol)
	// Set offset.
	offset += distance(beg, AnsiCharUtils::SkipLineBreak(eol, end));

	*this = std::move(line);
	return true;
}


_L_NS_END_NET_HTTP

