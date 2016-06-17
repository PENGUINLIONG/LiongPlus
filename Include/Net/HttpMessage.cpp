// File: HttpMessage.cpp
// Author: Rendong Liang (Liong)
#include "HttpMessage.hpp"
#include "../IO/MemoryStream.hpp"

namespace LiongPlus
{
	namespace Net
	{
		using namespace std;
		using std::swap;

		//
		// HttpResponseLine
		//

		HttpRequestLine::HttpRequestLine(HttpRequestLine&& instance)
			: HttpRequestLine()
		{
			swap(MajorVersion, instance.MajorVersion);
			swap(MinorVersion, instance.MinorVersion);
			swap(Method, instance.Method);
			swap(Path, instance.Path);
		}
		HttpRequestLine::HttpRequestLine(long major, long minor, string method, string path)
			: MajorVersion(major)
			, MinorVersion(minor)
			, Method(method)
			, Path(path)
		{
		}

		HttpRequestLine& HttpRequestLine::operator=(const HttpRequestLine& instance)
		{
			MajorVersion = instance.MajorVersion;
			MinorVersion = instance.MinorVersion;
			Method = instance.Method;
			Path = instance.Path;
			return *this;
		}
		HttpRequestLine& HttpRequestLine::operator=(HttpRequestLine&& instance)
		{
			swap(MajorVersion, instance.MajorVersion);
			swap(MinorVersion, instance.MinorVersion);
			swap(Method, instance.Method);
			swap(Path, instance.Path);
			return *this;
		}

		string HttpRequestLine::ToString() const
		{
			stringstream ss;
			ss << Method << ' ' << Path << "HTTP/" << MajorVersion << '.' << MinorVersion << '\n';
			return ss.str();
		}

		//
		// HttpResponseLine
		//

		HttpStatusLine::HttpStatusLine(HttpStatusLine&& instance)
			: HttpStatusLine()
		{
			swap(MajorVersion, instance.MajorVersion);
			swap(MinorVersion, instance.MinorVersion);
			swap(Status, instance.Status);
		}
		HttpStatusLine::HttpStatusLine(long major, long minor, string status)
			: MajorVersion(major)
			, MinorVersion(minor)
			, Status(status)
		{
		}

		HttpStatusLine& HttpStatusLine::operator=(const HttpStatusLine& instance)
		{
			MajorVersion = instance.MajorVersion;
			MinorVersion = instance.MinorVersion;
			Status = instance.Status;
			return *this;
		}
		HttpStatusLine& HttpStatusLine::operator=(HttpStatusLine&& instance)
		{
			swap(MajorVersion, instance.MajorVersion);
			swap(MinorVersion, instance.MinorVersion);
			swap(Status, instance.Status);
			return *this;
		}

		string HttpStatusLine::ToString() const
		{
			stringstream ss;
			ss << "HTTP/" << MajorVersion << '.' << MinorVersion << ' ' << Status << '\n';
			return ss.str();
		}

		//
		// HttpHeader
		//

		// Private
		
		void HttpHeader::AddElements(string& key, string& value)
		{
			_Headers[key] = value;
		}

		// Public

		const char
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

		HttpHeader& HttpHeader::operator=(const HttpHeader& instance)
		{
			_Headers = instance._Headers;
			return *this;
		}
		HttpHeader& HttpHeader::operator=(HttpHeader&& instance)
		{
			swap(*this, instance);
			return *this;
		}
		string& HttpHeader::operator[](string& key)
		{
			return _Headers[key];
		}

		bool HttpHeader::Contains(string& key) const
		{
			return (_Headers.find(key) != _Headers.end());
		}
		// This will return the boolean value which indicates whether the key/value pair is NOT existing.
		void HttpHeader::Remove(string& key)
		{
			_Headers.erase(key);
		}
		string HttpHeader::ToString() const
		{
			stringstream ss;
			for (auto& header : _Headers)
				ss << header.first << ": " << header.second << '\n';
			return ss.str();
		}

		//
		// HttpRequest
		//

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

		Buffer HttpRequest::ToBuffer() const
		{
			return forward<Buffer>(HttpMessage::ToBuffer(RequestLine, Header, Content));
		}

		Buffer HttpResponse::ToBuffer() const
		{
			return forward<Buffer>(HttpMessage::ToBuffer(StatusLine, Header, Content));
		}
	}
}
