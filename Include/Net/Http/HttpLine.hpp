// File: HttpLine.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include <string>
#include "_"
#include "../../Buffer.hpp"

_L_NS_BEG_NET_HTTP

namespace HttpMethods
{
	extern const char
		*Get, *Head, *Post, *Put, *Delete, *Trace, *Options, *Connect, *Patch;
}

class HttpLine
{
public:
	long MajorVersion = 0, MinorVersion = 0;

	HttpLine() = default;
	HttpLine(long major, long minor);

	virtual std::string ToString() const = 0;
};

class HttpRequestLine
	: public HttpLine
{
public:
	std::string Method, Path;

	HttpRequestLine() = default;
	HttpRequestLine(const HttpRequestLine&) = default;
	HttpRequestLine(HttpRequestLine&&);
	HttpRequestLine(long major, long minor, std::string method, std::string path);

	HttpRequestLine& operator=(const HttpRequestLine&);
	HttpRequestLine& operator=(HttpRequestLine&&);

	std::string ToString() const override;
	bool FromBuffer(Buffer& buffer, size_t& offset);
};

class HttpStatusLine
	: public HttpLine
{
public:
	long StatusCode = 0;
	std::string Reason;

	HttpStatusLine() = default;
	HttpStatusLine(const HttpStatusLine&) = default;
	HttpStatusLine(HttpStatusLine&&);
	HttpStatusLine(long major, long minor, long statusCode, std::string status);

	HttpStatusLine& operator=(const HttpStatusLine&);
	HttpStatusLine& operator=(HttpStatusLine&&);

	std::string ToString() const override;
	bool FromBuffer(Buffer buffer, size_t& offset);
};


_L_NS_END_NET_HTTP



