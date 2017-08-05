// File: HttpMessage.cpp
// Author: Rendong Liang (Liong)
#include <sstream>
#include "LiongPlus/Net/Http/HttpMessage.hpp"
#include "LiongPlus/AnsiCharUtils.hpp"

_L_NS_BEG_NET_HTTP
using namespace std;
using std::swap;

//
// HttpMessage
//

HttpMessage::HttpMessage(HttpMessage&& instance)
	: HttpMessage()
{
	swap(Header, instance.Header);
	swap(Content, instance.Content);
}
HttpMessage::HttpMessage(HttpHeader&& header, Buffer&& content)
	: Header(header)
	, Content(content)
{
}

Buffer HttpMessage::ToBuffer(const HttpLine& _line, const HttpHeader& _header, const Buffer& _content)
{
	string line = _line.ToString();
	string header = _header.ToString();

	Buffer buffer(line.length() + header.length() + 1 + _content.Length()); // +1 for the extra '\n'.
	memcpy(buffer.Field(), line.c_str(), line.length());
	memcpy(buffer.Field() + line.length(), header.c_str(), header.length());
	buffer.Field()[line.length() + header.length()] = '\n';
	memcpy(buffer.Field() + line.length() + header.length() + 1, _content.Field(), _content.Length());
	return buffer;
}

//
// HttpRequest
//

HttpRequest::HttpRequest(HttpRequest&& instance)
	: HttpRequest()
{
	swap(RequestLine, instance.RequestLine);
	swap(Header, instance.Header);
	swap(Content, instance.Content);
}
HttpRequest::HttpRequest(HttpRequestLine&& line, HttpHeader&& header, Buffer&& content)
	: HttpMessage(forward<HttpHeader>(header), forward<Buffer>(content))
	, RequestLine(line)
{
}

HttpRequest& HttpRequest::operator=(HttpRequest&& instance)
{
	swap(RequestLine, instance.RequestLine);
	swap(Header, instance.Header);
	swap(Content, instance.Content);
	return *this;
}

Buffer HttpRequest::ToBuffer() const
{
	return forward<Buffer>(HttpMessage::ToBuffer(RequestLine, Header, Content));
}

size_t HttpRequest::FromBuffer(Buffer& buffer, size_t offset)
{
	auto beg = buffer.Field() + offset;
	auto end = buffer.Field() + buffer.Length();
	auto pos = beg;

	if (!RequestLine.FromBuffer(buffer, offset))
		return 0;
	if (!Header.FromBuffer(buffer, offset))
		return 0;
	if (Header.Contains(EntityHeaders::ContentLength))
	{
		size_t length = std::stoull(Header[EntityHeaders::ContentLength]);
		Content = Buffer(length);
		buffer.CopyTo(Content.Field(), offset, buffer.Length() - offset);
	}
	return offset;
}

//
// HttpResponse
//

HttpResponse::HttpResponse(HttpResponse&& instance)
	: HttpResponse()
{
	swap(StatusLine, instance.StatusLine);
	swap(Header, instance.Header);
	swap(Content, instance.Content);
}
HttpResponse::HttpResponse(HttpStatusLine&& line, HttpHeader&& header, Buffer&& content)
	: HttpMessage(forward<HttpHeader>(header), forward<Buffer>(content))
	, StatusLine(line)
{
}

HttpResponse& HttpResponse::operator=(HttpResponse&& instance)
{
	swap(StatusLine, instance.StatusLine);
	swap(Header, instance.Header);
	swap(Content, instance.Content);
	return *this;
}

Buffer HttpResponse::ToBuffer() const
{
	return forward<Buffer>(HttpMessage::ToBuffer(StatusLine, Header, Content));
}

size_t HttpResponse::FromBuffer(Buffer& buffer, size_t offset)
{
	auto beg = buffer.Field() + offset;
	auto end = buffer.Field() + buffer.Length();
	auto pos = beg;

	if (!StatusLine.FromBuffer(buffer, offset))
		return 0;
	if (!Header.FromBuffer(buffer, offset))
		return 0;
	if (Header.Contains(EntityHeaders::ContentLength))
	{
		size_t length = std::stoull(Header[EntityHeaders::ContentLength]);
		Content = Buffer(length);
		if (length > buffer.Length() - offset)
			buffer.CopyTo(Content.Field(), offset, buffer.Length() - offset);
		else
			buffer.CopyTo(Content.Field(), offset, length);
	}
	return offset;
}
_L_NS_END_NET_HTTP
