// File: HttpHeader.hpp
// Author: Rendong Liang (Liong)
#pragma once

#include <string>
#include <map>
#include "_"
#include "../../Buffer.hpp"

_L_NS_BEG_NET_HTTP

namespace GeneralHeaders
{
	extern const char
		*CacheControl,
		*Connection,
		*Date,
		*Pragma,
		*Trailer,
		*TransferEncoding,
		*Upgrade,
		*Via,
		*Warning;
}
namespace RequestHeaders
{
	extern const char
		*Accept,
		*AcceptCharset,
		*AcceptEncoding,
		*AcceptLanguage,
		*Authorization,
		*Except,
		*From,
		*Host,
		*IfMatch,
		*IfModifiedSince,
		*IfNoneMatch,
		*IfUnmodifiedSince,
		*MaxForwards,
		*ProxyAuthorization,
		*Range,
		*Referer,
		*TE,
		*UserAgent;
}
namespace ResponseHeaders
{
	extern const char
		*AcceptRange,
		*Age,
		*ETag,
		*Location,
		*ProxyAuthenticate,
		*RetryAfter,
		*Server,
		*Vary,
		*WwwAuthenticate;
}
namespace EntityHeaders
{
	extern const char
		*Allow,
		*ContentEncoding,
		*ContentLanguage,
		*ContentLength,
		*ContentLocation,
		*ContentMd5,
		*ContentRange,
		*ContentType,
		*Expires,
		*LastModified;
}

class HttpHeader
{
private:
	std::map<std::string, std::string> _Headers;

	template<typename ... TArgs>
	void AddElements(const std::string& key, const std::string& value, TArgs& ... others)
	{
		_Headers[key] = value;
		AddElements(others ...);
	}
	void AddElements(const std::string& key, const std::string& value);
public:


	HttpHeader() = default;
	HttpHeader(const HttpHeader&) = default;
	HttpHeader(HttpHeader&& instance);
	template<typename ... TArgs>
	HttpHeader(const std::string& key, const std::string& value, TArgs ... args)
		: HttpHeader()
	{
		AddElements(key, value, (const std::string&)args ...);
	}

	HttpHeader& operator=(const HttpHeader& instance);
	HttpHeader& operator=(HttpHeader&& instance);
	std::string& operator[](const std::string& key);

	bool Contains(const std::string& key) const;
	// This will return the boolean value which indicates whether the key/value pair is NOT existing.
	void Remove(const std::string& key);
	std::string ToString() const;
	bool FromBuffer(const Buffer& buffer, size_t& offset);
};

_L_NS_END_NET_HTTP
