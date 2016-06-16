#include "HttpHeader.hpp"

namespace LiongPlus
{
	namespace Net
	{
		// Private
		
		void HttpHeader::AddElements(string& key, string& value)
		{
			_Headers[key] = value;
		}

		// Public

		const const char
			*HttpHeader::GeneralHeaders::CacheControl = "Cache-Control",
			*HttpHeader::GeneralHeaders::Connection = "Connection",
			*HttpHeader::GeneralHeaders::Date = "Date",
			*HttpHeader::GeneralHeaders::Pragma = "Pragma",
			*HttpHeader::GeneralHeaders::Trailer = "Trailer",
			*HttpHeader::GeneralHeaders::TransferEncoding = "Transfer-Encoding",
			*HttpHeader::GeneralHeaders::Upgrade = "Upgrade",
			*HttpHeader::GeneralHeaders::Via = "Via",
			*HttpHeader::GeneralHeaders::Warning = "Warning";
		const char
			*HttpHeader::RequestHeaders::Accept = "Accept",
			*HttpHeader::RequestHeaders::AcceptCharset = "Accept-Charset",
			*HttpHeader::RequestHeaders::AcceptEncoding = "Accept-Encoding",
			*HttpHeader::RequestHeaders::AcceptLanguage = "Accept-Language",
			*HttpHeader::RequestHeaders::Authorization = "Authorization",
			*HttpHeader::RequestHeaders::Except = "Except",
			*HttpHeader::RequestHeaders::From = "From",
			*HttpHeader::RequestHeaders::Host = "Host",
			*HttpHeader::RequestHeaders::IfMatch = "If-Match",
			*HttpHeader::RequestHeaders::IfModifiedSince = "If-Modified-Since",
			*HttpHeader::RequestHeaders::IfNoneMatch = "If-None-Match",
			*HttpHeader::RequestHeaders::IfUnmodifiedSince = "If-Unmodified-Since",
			*HttpHeader::RequestHeaders::MaxForwards = "Max-Forwards",
			*HttpHeader::RequestHeaders::ProxyAuthorization = "Proxy-Authorization",
			*HttpHeader::RequestHeaders::Range = "Range",
			*HttpHeader::RequestHeaders::Referer = "Referer",
			*HttpHeader::RequestHeaders::TE = "TE",
			*HttpHeader::RequestHeaders::UserAgent = "User-Agent";
		const char
			*HttpHeader::ResponseHeaders::AcceptRange = "Accept-Range",
			*HttpHeader::ResponseHeaders::Age = "Age",
			*HttpHeader::ResponseHeaders::ETag = "ETag",
			*HttpHeader::ResponseHeaders::Location = "Location",
			*HttpHeader::ResponseHeaders::ProxyAuthenticate = "Proxy-Authenticate",
			*HttpHeader::ResponseHeaders::RetryAfter = "Retry-After",
			*HttpHeader::ResponseHeaders::Server = "Server",
			*HttpHeader::ResponseHeaders::Vary = "Vary",
			*HttpHeader::ResponseHeaders::WwwAuthenticate = "WWW-Authenticate";
		const char
			*HttpHeader::EntityHeaders::Allow = "Allow",
			*HttpHeader::EntityHeaders::ContentEncoding = "Content-Encoding",
			*HttpHeader::EntityHeaders::ContentLanguage = "Content-Language",
			*HttpHeader::EntityHeaders::ContentLength = "Content-Length",
			*HttpHeader::EntityHeaders::ContentLocation = "Content-Location",
			*HttpHeader::EntityHeaders::ContentMd5 = "Content-MD5",
			*HttpHeader::EntityHeaders::ContentRange = "Content-Range",
			*HttpHeader::EntityHeaders::ContentType = "Content-Type",
			*HttpHeader::EntityHeaders::Expires = "Expires",
			*HttpHeader::EntityHeaders::LastModified = "Last-Modified";

		HttpHeader::HttpHeader(HttpHeader&& instance)
		{
			swap(*this, instance);
		}

		string& HttpHeader::operator=(const HttpHeader& instance)
		{
			_Headers = instance._Headers;
		}
		string& HttpHeader::operator=(HttpHeader&& instance)
		{
			swap(*this, instance);
		}
		string& HttpHeader::operator[](string& key)
		{
			return _Headers[key];
		}

		bool HttpHeader::Exists(string& key) const
		{
			return (_Headers.find(key) != _Headers.end());
		}
		// This will return the boolean value which indicates whether the key/value pair is NOT existing.
		bool HttpHeader::Remove(string& key)
		{
			_Headers.erase(key);
		}
		string& HttpHeader::ToString() const
		{
			stringstream ss;
			for (auto& header : _Headers)
				ss << header.first << ": " << header.second << std::endl;
			return ss.str();
		}
	}
}
