// File: HttpHeader.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include <string>
#include "_"
#include "../../Buffer.hpp"
#include "HttpHeader.hpp"
#include "HttpLine.hpp"

_L_NS_BEG_NET_HTTP

struct HttpMessage
{
protected:
	HttpMessage() = default;
	HttpMessage(const HttpMessage&) = default;
	HttpMessage(HttpMessage&&);
	HttpMessage(HttpHeader&& header, Buffer&& content);

	static Buffer ToBuffer(const HttpLine& _line, const HttpHeader& _header, const Buffer& _content);
public:
	HttpHeader Header;
	Buffer Content;

	virtual Buffer ToBuffer() const = 0;
};

struct HttpRequest
	: public HttpMessage
{
public:
	HttpRequestLine RequestLine;

	HttpRequest() = default;
	HttpRequest(const HttpRequest&) = default;
	HttpRequest(HttpRequest&&);
	HttpRequest(HttpRequestLine&& line, HttpHeader&& header, Buffer&& content);

	HttpRequest& operator=(const HttpRequest&) = default;
	HttpRequest& operator=(HttpRequest&&);

	Buffer ToBuffer() const override;
	// Return the length of non-content.
	size_t FromBuffer(Buffer& buffer, size_t offset);
};

struct HttpResponse
	: public HttpMessage
{
public:
	HttpStatusLine StatusLine;

	HttpResponse() = default;
	HttpResponse(const HttpResponse&) = default;
	HttpResponse(HttpResponse&&);
	HttpResponse(HttpStatusLine&& line, HttpHeader&& header, Buffer&& content);

	HttpResponse& operator=(const HttpResponse&) = default;
	HttpResponse& operator=(HttpResponse&&);

	Buffer ToBuffer() const override;
	// Return the length of non-content.
	size_t FromBuffer(Buffer& buffer, size_t offset);
};

_L_NS_END_NET_HTTP
