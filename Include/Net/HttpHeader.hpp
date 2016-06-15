// File: HttpHeader.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "../Fundamental.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;
		class HttpHeader
		{
		private:
			map<string, string> _Headers;
		public:
			struct GeneralHeaders
			{
				const string
					CacheControl = "Cache-Control",
					Connection = "Connection",
					Date = "Date",
					Pragma = "Pragma",
					Trailer = "Trailer",
					TransferEncoding = "Transfer-Encoding",
					Upgrade = "Upgrade",
					Via = "Via",
					Warning = "Warning";
			};
			struct RequestHeaders
			{
				const string
					Accept = "Accept",
					AcceptCharset = "Accept-Charset",
					AcceptEncoding = "Accept-Encoding",
					AcceptLanguage = "Accept-Language",
					Authorization = "Authorization",
					Except = "Except",
					From = "From",
					Host = "Host",
					IfMatch = "If-Match",
					IfModifiedSince = "If-Modified-Since",
					IfNoneMatch = "If-None-Match",
					IfUnmodifiedSince = "If-Unmodified-Since",
					MaxForwards = "Max-Forwards",
					ProxyAuthorization = "Proxy-Authorization",
					Range = "Range",
					Referer = "Referer",
					TE = "TE",
					UserAgent = "User-Agent";
			};
			struct ResponseHeaders
			{
				const string
					AcceptRange = "Accept-Range",
					Age = "Age",
					ETag = "ETag",
					Location = "Location",
					ProxyAuthenticate = "Proxy-Authenticate",
					RetryAfter = "Retry-After",
					Server = "Server",
					Vary = "Vary",
					WwwAuthenticate = "WWW-Authenticate";
			};
			struct EntityHeaders
			{
				const string
					Allow = "Allow",
					ContentEncoding = "Content-Encoding",
					ContentLanguage = "Content-Language",
					ContentLength = "Content-Length",
					ContentLocation = "Content-Location",
					ContentMd5 = "Content-MD5",
					ContentRange = "Content-Range",
					ContentType = "Content-Type",
					Expires = "Expires",
					LastModified = "Last-Modified";
			};

			HttpHeader();
			~HttpHeader();

			void Add(string& speifier, string& field);
			void Exists(string& specifier) const;
			int Remove(string&);
		};
	}
}