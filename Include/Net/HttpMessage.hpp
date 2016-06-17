// File: HttpHeader.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "../Fundamental.hpp"
#include "../Buffer.hpp"
#include "Socket.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;
		using std::swap;

		class HttpLine
		{
		public:
			virtual string ToString() const = 0;
		};

		struct HttpMethod
		{
			static const char
				*Get, *Head, *Post, *Put, *Delete, *Trace, *Options, *Connect, *Patch;
		};

		struct HttpRequestLine
			: public HttpLine
		{
		public:
			long MajorVersion, MinorVersion;
			string Method, Path;

			HttpRequestLine() = default;
			HttpRequestLine(const HttpRequestLine&) = default;
			HttpRequestLine(HttpRequestLine&&);
			HttpRequestLine(long major, long minor, string method, string path);

			HttpRequestLine& operator=(const HttpRequestLine&);
			HttpRequestLine& operator=(HttpRequestLine&&);

			string ToString() const override;
		};

		struct HttpStatusLine
			: public HttpLine
		{
		public:
			long MajorVersion, MinorVersion, StatusCode;
			string Status;

			HttpStatusLine() = default;
			HttpStatusLine(const HttpStatusLine&) = default;
			HttpStatusLine(HttpStatusLine&&);
			HttpStatusLine(long major, long minor, long statusCode, string status);

			HttpStatusLine& operator=(const HttpStatusLine&);
			HttpStatusLine& operator=(HttpStatusLine&&);
			
			string ToString() const override;
		};

		class HttpHeader
		{
		private:
			map<string, string> _Headers;

			template<typename ... TArgs>
			void AddElements(string& key, string& value, TArgs& ... others)
			{
				_Headers[key] = value;
				AddElements(others ...);
			}
			void AddElements(string& key, string& value);
		public:
			struct General
			{
				static const char
					*CacheControl,
					*Connection,
					*Date,
					*Pragma,
					*Trailer,
					*TransferEncoding,
					*Upgrade,
					*Via,
					*Warning;
			};
			struct Request
			{
				static const char
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
			};
			struct Response
			{
				static const char
					*AcceptRange,
					*Age,
					*ETag,
					*Location,
					*ProxyAuthenticate,
					*RetryAfter,
					*Server,
					*Vary,
					*WwwAuthenticate;
			};
			struct Entity
			{
				static const char
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
			};

			HttpHeader() = default;
			HttpHeader(const HttpHeader&) = default;
			HttpHeader(HttpHeader&& instance);
			template<typename ... TArgs>
			HttpHeader(string key, string value, TArgs ... args)
				: HttpHeader()
			{
				AddElements(key, value, (string)args ...);
			}

			HttpHeader& operator=(const HttpHeader& instance);
			HttpHeader& operator=(HttpHeader&& instance);
			string& operator[](string& key);

			bool Contains(string& key) const;
			// This will return the boolean value which indicates whether the key/value pair is NOT existing.
			void Remove(string& key);
			string ToString() const;
		};

		struct HttpMessage
		{
		protected:
			HttpMessage(HttpHeader header, Buffer content);

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

			HttpRequest(HttpHeader& header, HttpRequestLine& line, Buffer& Content);

			Buffer ToBuffer() const override;
		};

		struct HttpResponse
			: public HttpMessage
		{
		public:
			HttpStatusLine StatusLine;

			HttpResponse(HttpHeader& header, HttpStatusLine& line, Buffer& content);

			Buffer ToBuffer() const override;
			
		};
	}
}