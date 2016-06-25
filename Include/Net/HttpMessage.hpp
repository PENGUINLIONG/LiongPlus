// File: HttpHeader.hpp
// Author: Rendong Liang (Liong)
#pragma once
#include "../Fundamental.hpp"
#include "../Buffer.hpp"
#include "HttpUtils.hpp"
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
			long MajorVersion = 0, MinorVersion = 0;

			HttpLine() = default;
			HttpLine(long major, long minor);

			virtual string ToString() const = 0;
		};

		class HttpMethod
		{
		public:
			static const char
				*Get, *Head, *Post, *Put, *Delete, *Trace, *Options, *Connect, *Patch;
		};

		class HttpRequestLine
			: public HttpLine
		{
		public:
			string Method, Path;

			HttpRequestLine() = default;
			HttpRequestLine(const HttpRequestLine&) = default;
			HttpRequestLine(HttpRequestLine&&);
			HttpRequestLine(long major, long minor, string method, string path);

			HttpRequestLine& operator=(const HttpRequestLine&);
			HttpRequestLine& operator=(HttpRequestLine&&);

			string ToString() const override;
			bool FromBuffer(Buffer& buffer, size_t& offset);
		};

		class HttpStatusLine
			: public HttpLine
		{
		public:
			long StatusCode = 0;
			string Reason;

			HttpStatusLine() = default;
			HttpStatusLine(const HttpStatusLine&) = default;
			HttpStatusLine(HttpStatusLine&&);
			HttpStatusLine(long major, long minor, long statusCode, string status);

			HttpStatusLine& operator=(const HttpStatusLine&);
			HttpStatusLine& operator=(HttpStatusLine&&);
			
			string ToString() const override;
			bool FromBuffer(Buffer buffer, size_t& offset);
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
			string& operator[](const char* key);

			bool Contains(const char* key) const;
			// This will return the boolean value which indicates whether the key/value pair is NOT existing.
			void Remove(const char* key);
			string ToString() const;
			bool FromBuffer(Buffer& buffer, size_t& offset);
		};

		struct HttpMessage
		{
		protected:
			HttpMessage() = default;
			HttpMessage(const HttpMessage&) = delete;
			HttpMessage(HttpMessage&&) = delete;
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

			HttpRequest() = default;
			HttpRequest(const HttpRequest&) = default;
			HttpRequest(HttpRequest&&);
			HttpRequest(HttpHeader& header, HttpRequestLine& line, Buffer& Content);

			Buffer ToBuffer() const override;
			// Return length read.
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
			HttpResponse(HttpHeader& header, HttpStatusLine& line, Buffer& content);

			Buffer ToBuffer() const override;
			size_t FromBuffer(Buffer& buffer, size_t offset);
		};
	}
}