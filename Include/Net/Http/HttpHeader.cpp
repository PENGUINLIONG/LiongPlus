// File: HttpHeader.cpp
// Author: Rendong Liang (Liong)

#include <sstream>
#include "HttpHeader.hpp"
#include "../../AnsiCharUtils.hpp"

using namespace std;

_L_NS_BEG_NET_HTTP

// Private

void HttpHeader::AddElements(const string& key, const string& value)
{
	_Headers[key] = value;
}

// Public

namespace GeneralHeaders
{
	const char
		*CacheControl = "Cache-Control",
		*Connection = "Connection",
		*Date = "Date",
		*Pragma = "Pragma",
		*Trailer = "Trailer",
		*TransferEncoding = "Transfer-Encoding",
		*Upgrade = "Upgrade",
		*Via = "Via",
		*Warning = "Warning";
}
namespace RequestHeaders
{
	const char
		*Accept = "Accept",
		*AcceptCharset = "Accept-Charset",
		*AcceptEncoding = "Accept-Encoding",
		*AcceptLanguage = "Accept-Language",
		*Authorization = "Authorization",
		*Except = "Except",
		*From = "From",
		*Host = "Host",
		*IfMatch = "If-Match",
		*IfModifiedSince = "If-Modified-Since",
		*IfNoneMatch = "If-None-Match",
		*IfUnmodifiedSince = "If-Unmodified-Since",
		*MaxForwards = "Max-Forwards",
		*ProxyAuthorization = "Proxy-Authorization",
		*Range = "Range",
		*Referer = "Referer",
		*TE = "TE",
		*UserAgent = "User-Agent";
}
namespace ResponseHeaders
{
	const char
		*AcceptRange = "Accept-Range",
		*Age = "Age",
		*ETag = "ETag",
		*Location = "Location",
		*ProxyAuthenticate = "Proxy-Authenticate",
		*RetryAfter = "Retry-After",
		*Server = "Server",
		*Vary = "Vary",
		*WwwAuthenticate = "WWW-Authenticate";
}
namespace EntityHeaders
{
	const char
		*Allow = "Allow",
		*ContentEncoding = "Content-Encoding",
		*ContentLanguage = "Content-Language",
		*ContentLength = "Content-Length",
		*ContentLocation = "Content-Location",
		*ContentMd5 = "Content-MD5",
		*ContentRange = "Content-Range",
		*ContentType = "Content-Type",
		*Expires = "Expires",
		*LastModified = "Last-Modified";
}

HttpHeader::HttpHeader(HttpHeader&& instance)
{
	swap(_Headers, instance._Headers);
}

HttpHeader& HttpHeader::operator=(const HttpHeader& instance)
{
	_Headers = instance._Headers;
	return *this;
}
HttpHeader& HttpHeader::operator=(HttpHeader&& instance)
{
	swap(_Headers, instance._Headers);
	return *this;
}
string& HttpHeader::operator[](const string& key)
{
	return _Headers[key];
}

bool HttpHeader::Contains(const string& key) const
{
	return (_Headers.find(key) != _Headers.end());
	for (pair<string, string> pr : _Headers)
	{
		if (AnsiCharUtils::CaseInsensitiveEquals(pr.first, key))
			return true;
	}
	return false;
}

// This will return the boolean value which indicates whether the key/value pair is NOT existing.
void HttpHeader::Remove(const string& key)
{
	_Headers.erase(key);
}

string HttpHeader::ToString() const
{
	stringstream ss;
	for (pair<string, string> header : _Headers)
		ss << header.first << ": " << header.second << '\n';
	return ss.str();
}

bool HttpHeader::FromBuffer(const Buffer& buffer, size_t& offset)
{
	auto last = _Headers.end();
	auto beg = buffer.Field() + offset;
	auto end = buffer.Field() + buffer.Length();
	auto pos = beg;
	for (;;)
	{
		pos = AnsiCharUtils::SeekForNonWhiteSpace(pos, end);
		auto eol = AnsiCharUtils::SeekForLineBreak(pos, end);

		// At the end of each HTTP header, an empty line should exist.
		if (pos == eol) // This actually equals to the case of $divisor == $eol.
		{
			// There is no empty line, improper format.
			if (eol == end) return false;

			offset = AnsiCharUtils::SeekForNextLine(beg, eol) - beg;
			return true;
		}

		// If LWS (linear white space) presents, this line is the complement of the previous one.
		if (AnsiCharUtils::IsWhiteSpace(*beg))
		{
			auto nlws = AnsiCharUtils::SeekForNonWhiteSpace(beg, eol);
			if (nlws < eol)
			{
				if (last != _Headers.end()) (*last).second += string(nlws, eol);
				else return false;
			}
			continue;
		}

		auto pos_colon = AnsiCharUtils::SeekForChar(pos, eol, ':');
		if (pos_colon < eol)
		{
			string key(pos, AnsiCharUtils::SeekFor(pos, eol, AnsiCharUtils::IsWhiteSpace));
			string value(pos_colon, AnsiCharUtils::SeekFor(pos_colon, eol, AnsiCharUtils::IsWhiteSpace));
			if (Contains(key))
				_Headers[key] += _Headers[key] == "" ? value : ", " + value;
			else
				last = _Headers.emplace(key, value).first;
		}
		// Improper format.
		else return false;

		pos = AnsiCharUtils::SeekForNextLine(pos, end);
	}
}

_L_NS_END_NET_HTTP
