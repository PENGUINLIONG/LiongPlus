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

			template<typename ... TArgs>
			void AddElements(string& key, string& value, TArgs ... others)
			{
				_Headers[key] = value;
				ConnectElements(others ...);
			}
			void AddElements(string& key, string& value);
		public:
			struct GeneralHeaders
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
			struct RequestHeaders
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
			struct ResponseHeaders
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
			struct EntityHeaders
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
			HttpHeader(HttpHeader&& instance)
			{
				swap(*this, instance);
			}
			template<size_t TSize>
			HttpHeader(initializer_list<string> list)
				: HttpHeader()
			{
				AddElements(list);
			}

			string& operator=(const HttpHeader& instance);
			string& operator=(HttpHeader&& instance);
			string& operator[](string& key);

			bool Exists(string& key) const;
			// This will return the boolean value which indicates whether the key/value pair is NOT existing.
			bool Remove(string& key);
			string& ToString() const;
		};
	}
}